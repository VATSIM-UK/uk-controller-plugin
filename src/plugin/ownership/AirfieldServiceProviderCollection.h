#pragma once

namespace UKControllerPlugin::Controller {
    class ActiveCallsign;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Ownership {
    class ServiceProvision;

    /**
     * A collection of service providers at an airfield,
     * e.g providing delivery, tower etc.
     */
    class AirfieldServiceProviderCollection
    {
        public:
        [[nodiscard]] auto
        SetProvidersForAirfield(const std::string& icao, std::vector<std::shared_ptr<ServiceProvision>> providers);
        [[nodiscard]] auto DeliveryControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto AirfieldHasDeliveryProvider(const std::string& icao) const -> bool;
        [[nodiscard]] auto GetServiceProviders(const std::string& icao) const
            -> std::vector<std::shared_ptr<ServiceProvision>>&;
        [[nodiscard]] auto ApproachControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto TowerControlProvidedByUser(const std::string& icao) const -> bool;
        void Flush();

        private:
        const std::vector<std::shared_ptr<ServiceProvision>> noProviders;

        // Map of airfield ICAO to who's providing services there
        std::map<std::string, std::vector<std::shared_ptr<ServiceProvision>>> serviceProviders;
    };
} // namespace UKControllerPlugin::Ownership
