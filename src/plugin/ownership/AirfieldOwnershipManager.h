#pragma once

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
        class AirfieldModel;
    } // namespace Airfield
    namespace Controller {
        class ActiveCallsign;
        class ActiveCallsignCollection;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Ownership {
    struct ServiceProvision;
    class AirfieldServiceProviderCollection;

    /*
        A class for managing who "owns" a particular airfield. This can then be used
        to determine whether or not the client should be setting initial altitudes or
        requesting squawks. Naturally, there are server side checks for this too,
        but nobody likes a hammered server.
    */
    class AirfieldOwnershipManager
    {
        public:
        AirfieldOwnershipManager(
            std::shared_ptr<AirfieldServiceProviderCollection> serviceProviders,
            const UKControllerPlugin::Airfield::AirfieldCollection& airfields,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns);
        void Flush();
        [[nodiscard]] auto GetOwnedAirfields(const std::string& callsign) const
            -> std::vector<std::shared_ptr<UKControllerPlugin::Airfield::AirfieldModel>>;
        void RefreshOwner(const std::string& icao);
        [[nodiscard]] auto GetProviders() const -> const AirfieldServiceProviderCollection&;

        private:
        [[nodiscard]] static auto ServiceProviderMatchingConditionExists(
            const std::vector<std::shared_ptr<ServiceProvision>>& providers,
            const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate) -> bool;
        [[nodiscard]] static auto MapServiceProvisionToString(const std::shared_ptr<ServiceProvision>& provision)
            -> std::string;
        static void LogNewServiceProvision(const std::string& icao, const std::shared_ptr<ServiceProvision>& provision);
        static void
        LogRemovedServiceProvision(const std::string& icao, const std::shared_ptr<ServiceProvision>& provision);
        [[nodiscard]] auto GetActiveControllersInAirfieldTopdownOrder(
            const std::string& icao, const std::shared_ptr<Airfield::AirfieldModel>& airfield) ->
            std::vector<std::string>;
        [[nodiscard]] auto GetServiceProvidersAtAirfield(const std::vector<std::string>& controllers)
            -> std::vector<std::shared_ptr<ServiceProvision>>;
        void
        LogProviderChanges(const std::string& icao, const std::vector<std::shared_ptr<ServiceProvision>>& newProviders);

        // All the active users.
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        // Collection of all airfields
        const UKControllerPlugin::Airfield::AirfieldCollection& airfields;

        // Who's providing services
        const std::shared_ptr<AirfieldServiceProviderCollection> serviceProviders;
    };
} // namespace UKControllerPlugin::Ownership
