#include "AirfieldServiceProviderCollection.h"
#include "ServiceProvision.h"
#include "controller/ActiveCallsign.h"

namespace UKControllerPlugin::Ownership {

    void AirfieldServiceProviderCollection::SetProvidersForAirfield(
        const std::string& icao, std::vector<std::shared_ptr<ServiceProvision>> providers)
    {
        this->serviceProviders[icao] = std::move(providers);
    }

    auto AirfieldServiceProviderCollection::DeliveryControlProvidedByUser(const std::string& icao) const -> bool
    {
        return this->ServiceProvidedAtAirfieldByUser(icao, ServiceType::Delivery);
    }

    auto AirfieldServiceProviderCollection::AirfieldHasDeliveryProvider(const std::string& icao) const -> bool
    {
        return this->ServiceProvidedAtAirfield(icao, ServiceType::Delivery);
    }

    auto AirfieldServiceProviderCollection::FinalApproachControlProvidedByUser(const std::string& icao) const -> bool
    {
        return this->ServiceProvidedAtAirfieldByUser(icao, ServiceType::FinalApproach);
    }

    auto AirfieldServiceProviderCollection::ApproachControlProvidedByUser(const std::string& icao) const -> bool
    {
        return this->ServiceProvidedAtAirfieldByUser(icao, ServiceType::Approach);
    }

    auto AirfieldServiceProviderCollection::TowerControlProvidedByUser(const std::string& icao) const -> bool
    {
        return this->ServiceProvidedAtAirfieldByUser(icao, ServiceType::Tower);
    }

    void AirfieldServiceProviderCollection::Flush()
    {
        this->serviceProviders.clear();
    }

    auto AirfieldServiceProviderCollection::GetServiceProviders(const std::string& icao) const
        -> const std::vector<std::shared_ptr<ServiceProvision>>&
    {
        return this->serviceProviders.count(icao) != 0 ? this->serviceProviders.at(icao) : this->noProviders;
    }

    auto AirfieldServiceProviderCollection::GetFirstServiceProviderMatchingCondition(
        const std::vector<std::shared_ptr<ServiceProvision>>& providers,
        const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate) const
        -> const std::shared_ptr<ServiceProvision>&
    {
        auto provider = std::find_if(providers.cbegin(), providers.cend(), predicate);
        return provider == providers.cend() ? this->invalidProvider : *provider;
    }

    auto AirfieldServiceProviderCollection::HasServiceProviderMatchingCondition(
        const std::vector<std::shared_ptr<ServiceProvision>>& providers,
        const std::function<bool(const std::shared_ptr<ServiceProvision>&)>& predicate) const -> bool
    {
        return this->GetFirstServiceProviderMatchingCondition(providers, predicate) != nullptr;
    }

    auto
    AirfieldServiceProviderCollection::ServiceProvidedAtAirfieldByUser(const std::string& icao, ServiceType type) const
        -> bool
    {
        return this->serviceProviders.count(icao) != 0 &&
               this->HasServiceProviderMatchingCondition(
                   this->serviceProviders.at(icao),
                   [&type](const std::shared_ptr<ServiceProvision>& provision) -> bool {
                       return provision->serviceProvided == type && provision->controller->GetIsUser();
                   });
    }

    auto AirfieldServiceProviderCollection::ServiceProvidedAtAirfield(const std::string& icao, ServiceType type) const
        -> bool
    {
        return this->serviceProviders.count(icao) != 0 &&
               this->HasServiceProviderMatchingCondition(
                   this->serviceProviders.at(icao),
                   [&type](const std::shared_ptr<ServiceProvision>& provision) -> bool {
                       return provision->serviceProvided == type;
                   });
    }

    void AirfieldServiceProviderCollection::FlushForAirfield(const std::string& icao)
    {
        this->serviceProviders.erase(icao);
    }

    auto AirfieldServiceProviderCollection::GetAirfieldsWhereUserIsProvidingDelivery() const -> std::vector<std::string>
    {
        std::vector<std::string> airfields;
        for (const auto& providersAtAirfield : this->serviceProviders) {
            if (this->DeliveryControlProvidedByUser(providersAtAirfield.first)) {
                airfields.push_back(providersAtAirfield.first);
            }
        }

        return airfields;
    }

    auto AirfieldServiceProviderCollection::DeliveryProviderForAirfield(const std::string& icao) const
        -> const std::shared_ptr<ServiceProvision>&
    {
        if (this->serviceProviders.count(icao) == 0) {
            return this->invalidProvider;
        }

        return this->GetFirstServiceProviderMatchingCondition(
            this->serviceProviders.at(icao), [](const std::shared_ptr<ServiceProvision>& provision) -> bool {
                return provision->serviceProvided == ServiceType::Delivery;
            });
    }

    auto
    AirfieldServiceProviderCollection::GetProvidersForServiceAtAirfield(const std::string& icao, ServiceType type) const
        -> std::vector<std::shared_ptr<ServiceProvision>>
    {
        if (this->serviceProviders.count(icao) == 0) {
            return this->noProviders;
        }

        std::vector<std::shared_ptr<ServiceProvision>> providers;
        std::copy_if(
            this->serviceProviders.at(icao).cbegin(),
            this->serviceProviders.at(icao).cend(),
            std::back_inserter(providers),
            [&type](const std::shared_ptr<ServiceProvision>& provider) -> bool {
                return provider->serviceProvided == type;
            });

        return providers;
    }
} // namespace UKControllerPlugin::Ownership
