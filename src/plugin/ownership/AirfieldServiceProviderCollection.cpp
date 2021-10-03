#include "AirfieldServiceProviderCollection.h"

namespace UKControllerPlugin::Ownership {

    auto AirfieldServiceProviderCollection::SetProvidersForAirfield(
        const std::string& icao, std::vector<std::shared_ptr<ServiceProvision>> providers)
    {
        this->serviceProviders[icao] = std::move(providers);
    }
    
    auto AirfieldServiceProviderCollection::DeliveryControlProvidedByUser(const std::string& icao) const -> bool
    {
        return false;
    }
} // namespace UKControllerPlugin::Ownership
