#pragma once

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
        class AirfieldModel;
    } // namespace Airfield
    namespace Controller {
        class ActiveCallsign;
        class ActiveCallsignCollection;
        class ControllerPosition;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Ownership {
    struct ServiceProvision;

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
            const UKControllerPlugin::Airfield::AirfieldCollection& airfields,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns);
        ~AirfieldOwnershipManager();
        [[nodiscard]] auto
        AirfieldOwnedBy(const std::string& icao, const UKControllerPlugin::Controller::ActiveCallsign& position) const
            -> bool;
        [[nodiscard]] auto AirfieldOwnedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto AirfieldHasOwner(const std::string& icao) const -> bool;
        [[nodiscard]] auto ApproachControlProvidedByUser(const std::string& icao) const -> bool;
        [[nodiscard]] auto TowerControlProvidedByUser(const std::string& icao) const -> bool;
        void Flush();
        [[nodiscard]] auto GetOwner(const std::string& icao) const
            -> const UKControllerPlugin::Controller::ActiveCallsign&;
        [[nodiscard]] auto GetOwnedAirfields(const std::string& callsign) const
            -> std::vector<std::reference_wrapper<const UKControllerPlugin::Airfield::AirfieldModel>>;
        void RefreshOwner(const std::string& icao);
        [[nodiscard]] auto NotFoundCallsign() const -> Controller::ActiveCallsign&;

        private:
        [[nodiscard]] auto GetDeliveryServiceProviderForAirfield(const std::string& icao) const ->
            std::shared_ptr<ServiceProvision>;
        [[nodiscard]] static auto ServiceProviderMatchingConditionExists(
            const std::vector<std::shared_ptr<ServiceProvision>>& providers,
            const std::function<bool(const std::shared_ptr<ServiceProvision>& provider)>& predicate
        ) -> bool;
        [[nodiscard]] static auto MapServiceProvisionToString(const std::shared_ptr<ServiceProvision>& provision) ->
            std::string;
        static void LogNewServiceProvision(const std::string& icao, const std::shared_ptr<ServiceProvision>& provision);
        static void LogRemovedServiceProvision(const std::string& icao, const std::shared_ptr<ServiceProvision>&
            provision);
        constexpr static const double INVALID_FREQUENCY = 199.998;

        // A controller position to return when a lookup is done but the callsign cant be found
        const std::unique_ptr<UKControllerPlugin::Controller::ControllerPosition> notFoundControllerPosition;

        // A callsign to return when a lookup is done but the callsign cant be found
        const std::unique_ptr<UKControllerPlugin::Controller::ActiveCallsign> notFoundCallsign;

        // All the active users.
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        // Collection of all airfields
        const UKControllerPlugin::Airfield::AirfieldCollection& airfields;

        // Map of airfield ICAO to who's providing services there
        std::map<std::string, std::vector<std::shared_ptr<ServiceProvision>>> serviceProviders;
    };
} // namespace UKControllerPlugin::Ownership
