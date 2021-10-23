#include "AirfieldOwnershipManager.h"
#include "AirfieldServiceProviderCollection.h"
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
        std::shared_ptr<AirfieldServiceProviderCollection> serviceProviders,
        const AirfieldCollection& airfields,
        const ActiveCallsignCollection& activeCallsigns)
        : activeCallsigns(activeCallsigns), airfields(airfields), serviceProviders(std::move(serviceProviders))
    {
    }

    /*
        Sets no owner on all airfields.
    */
    void AirfieldOwnershipManager::Flush()
    {
        this->serviceProviders->Flush();
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

        for (const auto& airfield : this->serviceProviders->GetAirfieldsWhereUserIsProvidingDelivery()) {
            ownedAirfields.emplace_back(this->airfields.FetchAirfieldByIcao(airfield));
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

        const auto activeControllers = this->GetActiveControllersInAirfieldTopdownOrder(icao, topdownOrder);
        if (activeControllers.empty()) {
            LogInfo("Airfield " + icao + " is now uncontrolled");
            this->serviceProviders->FlushForAirfield(icao);
            return;
        }

        const auto newServiceProviders = this->GetServiceProvidersAtAirfield(activeControllers);
        this->LogProviderChanges(icao, newServiceProviders);
        this->serviceProviders->SetProvidersForAirfield(icao, newServiceProviders);
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
        default:
            return "Invalid";
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

    auto AirfieldOwnershipManager::GetActiveControllersInAirfieldTopdownOrder(
        const std::string& icao, const std::vector<std::string>& topDownOrder) -> std::vector<std::string>
    {
        std::vector<std::string> activeControllers;
        for (const auto& controller : topDownOrder) {
            if (this->activeCallsigns.PositionActive(controller)) {
                activeControllers.push_back(controller);
            }
        }

        return activeControllers;
    }

    /**
     * This method has the following rationalle:
     *
     * - The first controller in the top-down order will be providing delivery services.
     * - There may be multiple Ground controllers. All of them provide ground services, but only one of them,
     * the first one, will be covering Delivery.
     * - There may be multiple Tower controllers, but only the first one in the top-down order (ie the primary
     * position) will be providing DEL/GND in the absence of those controllers.
     * - There may be multiple Approach controllers. Only the first approach controller in the top-down order
     * will be providing final approach services and below.
     * - If one Approach controller is online, they assume both Final approach and Approach services, regardless
     * of whether other controllers (e.g. Enroute) is online.
     * - Enroute controllers may provide all services down to the first position to be covered.
     * - At all levels apart from Delivery and Final Approach, multiple controllers of the same type can provide
     * the same service.
     */
    auto AirfieldOwnershipManager::GetServiceProvidersAtAirfield(const std::vector<std::string>& controllers)
        -> std::vector<std::shared_ptr<ServiceProvision>>
    {
        // Generate the new service providers
        std::vector<std::shared_ptr<ServiceProvision>> serviceProviders;
        for (const auto& controller : controllers) {
            const auto& leadCallsign = this->activeCallsigns.GetLeadCallsignForPosition(controller);
            const auto& normalisedPosition = leadCallsign.GetNormalisedPosition();

            // If nobody's providing any services yet, this controller is providing delivery.
            if (serviceProviders.empty()) {
                serviceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Delivery, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide GND, skip this.
            if (!normalisedPosition.ProvidesGroundServices()) {
                continue;
            }

            // If nobody's providing ground services yet, this controller is.
            if (normalisedPosition.IsGround() ||
                !ServiceProviderMatchingConditionExists(
                    serviceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Ground;
                    })) {
                serviceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Ground, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide TWR, skip this.
            if (!normalisedPosition.ProvidesTowerServices()) {
                continue;
            }

            // Only TWR controllers (or APP+ if there aren't any TWR's) can provide TWR services.
            if (normalisedPosition.IsTower() ||
                !ServiceProviderMatchingConditionExists(
                    serviceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Tower;
                    })) {
                serviceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Tower, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // If controller can't provide APP, skip this.
            if (!normalisedPosition.ProvidesApproachServices()) {
                continue;
            }

            // The first controller we find that can provide APP is the final phase controller
            if (!ServiceProviderMatchingConditionExists(
                    serviceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::FinalApproach;
                    })) {
                serviceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::FinalApproach, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }

            // Enroute and APP can provide generic app control
            if (normalisedPosition.IsApproach() ||
                !ServiceProviderMatchingConditionExists(
                    serviceProviders, [](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == ServiceType::Approach;
                    })) {
                serviceProviders.push_back(std::make_shared<ServiceProvision>(
                    ServiceType::Approach, std::make_shared<Controller::ActiveCallsign>(leadCallsign)));
            }
        }

        return serviceProviders;
    }

    void AirfieldOwnershipManager::LogProviderChanges(
        const std::string& icao, const std::vector<std::shared_ptr<ServiceProvision>>& newProviders)
    {
        const auto& currentProviders = this->serviceProviders->GetServiceProviders(icao);
        for (const auto& serviceProvider : newProviders) {
            if (!ServiceProviderMatchingConditionExists(
                    currentProviders, [&serviceProvider](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == serviceProvider->serviceProvided &&
                               *provider->controller == *serviceProvider->controller;
                    })) {
                LogNewServiceProvision(icao, serviceProvider);
            }
        }

        for (const auto& serviceProvider : currentProviders) {
            if (!ServiceProviderMatchingConditionExists(
                    newProviders, [&serviceProvider](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                        return provider->serviceProvided == serviceProvider->serviceProvided &&
                               *provider->controller == *serviceProvider->controller;
                    })) {
                LogRemovedServiceProvision(icao, serviceProvider);
            }
        }
    }

    auto AirfieldOwnershipManager::GetProviders() const -> const AirfieldServiceProviderCollection&
    {
        return *this->serviceProviders;
    }
} // namespace UKControllerPlugin::Ownership
