#include "AirfieldOwnershipManager.h"
#include "ServiceProvision.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin::Ownership {

    AirfieldOwnershipManager::AirfieldOwnershipManager(
        const AirfieldCollection& airfields, const ActiveCallsignCollection& activeCallsigns)
        : notFoundControllerPosition(
              std::make_unique<ControllerPosition>(-1, "", INVALID_FREQUENCY, std::vector<std::string>{}, true, false)),
          notFoundCallsign(std::make_unique<ActiveCallsign>("", "", *this->notFoundControllerPosition)),
          activeCallsigns(activeCallsigns), airfields(airfields)
    {
    }

    AirfieldOwnershipManager::~AirfieldOwnershipManager() = default;

    /*
        Returns true if an airfield is "owned" by the given controller, false otherwise.
    */
    auto AirfieldOwnershipManager::AirfieldOwnedBy(const std::string& icao, const ActiveCallsign& position) const
        -> bool
    {
        auto owner = this->GetDeliveryServiceProviderForAirfield(icao);
        return owner != nullptr && *owner->controller == position;
    }

    /*
        Returns true if an airfield is owned by the user.
    */
    auto AirfieldOwnershipManager::AirfieldOwnedByUser(const std::string& icao) const -> bool
    {
        return this->activeCallsigns.UserHasCallsign() &&
               this->AirfieldOwnedBy(icao, this->activeCallsigns.GetUserCallsign());
    }

    /*
        Returns true if the airfield has an owner.
    */
    auto AirfieldOwnershipManager::AirfieldHasOwner(const std::string& icao) const -> bool
    {
        return this->GetDeliveryServiceProviderForAirfield(icao) != nullptr;
    }

    /*
        Sets no owner on all airfields.
    */
    void AirfieldOwnershipManager::Flush()
    {
        this->serviceProviders.clear();
    }

    /*
        Returns the controller that owns a given airfield.
    */
    auto AirfieldOwnershipManager::GetOwner(const std::string& icao) const -> const ActiveCallsign&
    {
        auto provider = this->GetDeliveryServiceProviderForAirfield(icao);
        return provider == nullptr ? *this->notFoundCallsign : *provider->controller;
    }

    /*
        Returns a set of the airfields owned by a given contrroller.
    */
    auto AirfieldOwnershipManager::GetOwnedAirfields(const std::string& callsign) const
        -> std::vector<std::reference_wrapper<const AirfieldModel>>
    {
        std::vector<std::reference_wrapper<const AirfieldModel>> ownedAirfields;

        if (!this->activeCallsigns.CallsignActive(callsign)) {
            return ownedAirfields;
        }

        for (auto it = this->serviceProviders.cbegin(); it != this->serviceProviders.cend(); ++it) {
            if (ServiceProviderMatchingConditionExists(
                    it->second, [callsign](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->controller->GetCallsign() == callsign &&
                               provider->serviceProvided == ServiceType::Delivery;
                    })) {
                ownedAirfields.emplace_back(this->airfields.FetchAirfieldByIcao(it->first));
            }
        }

        return ownedAirfields;
    }

    /*
        Updates the owner of a given airfield.
    */
    void AirfieldOwnershipManager::RefreshOwner(const std::string& icao)
    {
        std::vector<std::string> topdownOrder;
        try {
            topdownOrder = this->airfields.FetchAirfieldByIcao(icao).GetOwnershipPresedence();
        } catch (std::out_of_range&) {
            // Nothing we can do if we can't find the airfield.
            return;
        }

        std::vector<std::string> activeControllers;

        // First lets work out who we've got online here
        for (const auto& controller : topdownOrder) {
            if (this->activeCallsigns.PositionActive(controller)) {
                activeControllers.push_back(controller);
            }
        }

        // Nobody's active, so we're done here
        if (activeControllers.empty()) {
            LogInfo("Airfield " + icao + " is now uncontrolled");
            this->serviceProviders.erase(icao);
            return;
        }

        // Generate the new service providers
        std::vector<std::shared_ptr<ServiceProvision>> newServiceProviders;
        for (const auto& controller : activeControllers) {
            const auto& leadCallsign = this->activeCallsigns.GetLeadCallsignForPosition(controller);
            const auto& normalisedPosition = leadCallsign.GetNormalisedPosition();

            // If nobody's providing any services yet, this controller is.
            if (newServiceProviders.empty()) {
                newServiceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Delivery, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide GND, skip this.
            if (!normalisedPosition.ProvidesGroundServices()) {
                continue;
            }

            // If nobody's providing ground services yet, this controller is.
            if (!ServiceProviderMatchingConditionExists(
                    newServiceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Ground;
                    })) {
                newServiceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Ground, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide TWR, skip this.
            if (!normalisedPosition.ProvidesTowerServices()) {
                continue;
            }

            // Only TWR controllers (or APP+ if there aren't any TWR's) can provide TWR services.
            if (normalisedPosition.IsTower() ||
                !ServiceProviderMatchingConditionExists(
                    newServiceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Tower;
                    })) {
                newServiceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Tower, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide APP, skip this.
            if (!normalisedPosition.ProvidesApproachServices()) {
                continue;
            }

            // The first controller we find that can provide APP is the final phase controller
            if (!ServiceProviderMatchingConditionExists(
                    newServiceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::FinalApproach;
                    })) {
                newServiceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::FinalApproach, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // Enroute and APP can provide generic app control
            if (normalisedPosition.IsApproach() ||
                !ServiceProviderMatchingConditionExists(
                    newServiceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Approach;
                    })) {
                newServiceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Approach, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }
        }

        // Phew, we've worked out who's doing what role. Now for the log messages.
        for (const auto& serviceProvider : newServiceProviders) {
            if (!ServiceProviderMatchingConditionExists(
                    this->serviceProviders[icao],
                    [&serviceProvider](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == serviceProvider->serviceProvided &&
                               *provider->controller == *serviceProvider->controller;
                    })) {
                LogNewServiceProvision(icao, serviceProvider);
            }
        }

        for (const auto& serviceProvider : this->serviceProviders[icao]) {
            if (!ServiceProviderMatchingConditionExists(
                    newServiceProviders, [&serviceProvider](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == serviceProvider->serviceProvided &&
                               *provider->controller == *serviceProvider->controller;
                    })) {
                LogNewServiceProvision(icao, serviceProvider);
            }
        }

        // Now replace the controllers
        this->serviceProviders[icao] = std::move(newServiceProviders);
    }

    auto AirfieldOwnershipManager::NotFoundCallsign() const -> Controller::ActiveCallsign&
    {
        return *this->notFoundCallsign;
    }

    auto AirfieldOwnershipManager::ServiceProviderMatchingConditionExists(
        const std::vector<std::shared_ptr<ServiceProvision>>& providers,
        const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate) -> bool
    {
        return std::find_if(providers.begin(), providers.end(), predicate) != providers.cend();
    }

    auto AirfieldOwnershipManager::MapServiceProvisionToString(const std::shared_ptr<ServiceProvision>& provision)
        -> std::string
    {
        switch (provision->serviceProvided) {
        case ServiceType::Delivery:
            return "Delivery";
        case ServiceType::Ground:
            return "Ground";
        case ServiceType::Tower:
            return "Tower";
        case ServiceType::FinalApproach:
            return "Final Approach";
        case ServiceType::Approach:
            return "Approach";
        }

        return "Unknown";
    }

    void AirfieldOwnershipManager::LogNewServiceProvision(
        const std::string& icao, const std::shared_ptr<ServiceProvision>& provision)
    {
        LogInfo(
            provision->controller->GetCallsign() + " is now providing " + MapServiceProvisionToString(provision) +
            " services at " + icao);
    }

    void AirfieldOwnershipManager::LogRemovedServiceProvision(
        const std::string& icao, const std::shared_ptr<ServiceProvision>& provision)
    {
        LogInfo(
            provision->controller->GetCallsign() + " is no longer providing " + MapServiceProvisionToString(provision) +
            " services at " + icao);
    }

    auto AirfieldOwnershipManager::GetDeliveryServiceProviderForAirfield(const std::string& icao) const
        -> std::shared_ptr<ServiceProvision>
    {
        if (this->serviceProviders.count(icao) == 0) {
            return nullptr;
        }

        const auto& providerForAirfield = this->serviceProviders.at(icao);
        auto provider = std::find_if(
            providerForAirfield.begin(),
            providerForAirfield.end(),
            [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                return provider->serviceProvided == ServiceType::Delivery;
            });
        return provider == providerForAirfield.cend() ? nullptr : *provider;
    }
} // namespace UKControllerPlugin::Ownership
