#pragma once
#include "ServiceType.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Ownership {
    struct ServiceProvision;

    /**
     * A collection of service providers at an airfield,
     * e.g providing delivery, tower etc.
     */
    class AirfieldServiceProviderCollection
    {
        public:
        void SetProvidersForAirfield(const std::string& icao, std::vector<std::shared_ptr<ServiceProvision>> providers);
        [[nodiscard]] auto GetAirfieldsWhereUserIsProvidingDelivery() const -> std::vector<std::string>;
        [[nodiscard]] auto GetAirfieldsWhereUserProvidingServices(ServiceType service) const
            -> std::vector<std::string>;
        [[nodiscard]] auto DeliveryProviderForAirfield(const std::string& icao) const
            -> const std::shared_ptr<ServiceProvision>&;
        [[nodiscard]] auto AirfieldHasDeliveryProvider(const std::string& icao) const -> bool;
        [[nodiscard]] auto DeliveryControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto GroundControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto FinalApproachControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto ApproachControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto TowerControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto GetServiceProviders(const std::string& icao) const
            -> const std::vector<std::shared_ptr<ServiceProvision>>&;
        void Flush();
        void FlushForAirfield(const std::string& icao);
        [[nodiscard]] auto ServiceProvidedAtAirfield(const std::string& icao, ServiceType type) const -> bool;
        [[nodiscard]] auto GetProvidersForServiceAtAirfield(const std::string& icao, ServiceType type) const
            -> std::vector<std::shared_ptr<ServiceProvision>>;

        private:
        [[nodiscard]] auto ServiceProvidedAtAirfieldByUser(const std::string& icao, ServiceType type) const -> bool;
        [[nodiscard]] auto GetFirstServiceProviderMatchingCondition(
            const std::vector<std::shared_ptr<ServiceProvision>>& providers,
            const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate) const
            -> const std::shared_ptr<ServiceProvision>&;
        [[nodiscard]] auto HasServiceProviderMatchingCondition(
            const std::vector<std::shared_ptr<ServiceProvision>>& providers,
            const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate) const -> bool;
        const std::vector<std::shared_ptr<ServiceProvision>> noProviders;

        const std::shared_ptr<ServiceProvision> invalidProvider = nullptr;

        // Map of airfield ICAO to who's providing services there
        std::map<std::string, std::vector<std::shared_ptr<ServiceProvision>>> serviceProviders;
    };
} // namespace UKControllerPlugin::Ownership
