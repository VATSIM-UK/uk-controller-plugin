#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace Integration {
        class OutboundIntegrationEventHandler;
    } // namespace Integration
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;
    class MissedApproachAudioAlert;

    /**
     * Triggers a missed approach message to everyone.
     */
    class TriggerMissedApproach
    {
        public:
        TriggerMissedApproach(
            std::shared_ptr<MissedApproachCollection> missedApproaches,
            Windows::WinApiInterface& windowsApi,
            const Api::ApiInterface& api,
            const Ownership::AirfieldServiceProviderCollection& serviceProviders,
            std::shared_ptr<const MissedApproachAudioAlert> audioAlert,
            const Integration::OutboundIntegrationEventHandler& integrationEvents
        );
        void Trigger(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const;

        private:
        [[nodiscard]] static auto AircraftElegibleForMissedApproach(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;
        [[nodiscard]] auto UserCanTrigger(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> bool;
        [[nodiscard]] auto Confirm(const std::string& callsign) const -> bool;
        [[nodiscard]] static auto ResponseValid(const nlohmann::json& responseData) -> bool;
        [[nodiscard]] auto AlreadyActive(const std::string& callsign) const -> bool;
        void TriggerMissedApproachInApi(const std::string& callsign) const;

        // All the missed approaches
        const std::shared_ptr<MissedApproachCollection> missedApproaches;

        // For the confirmation dialog
        Windows::WinApiInterface& windowsApi;

        // For sending events to other plugins
        const Api::ApiInterface& api;

        // Which controllers are online
        const Ownership::AirfieldServiceProviderCollection& serviceProviders;

        // Used for alerting the controller to the missed approach
        const std::shared_ptr<const MissedApproachAudioAlert> audioAlert;
        
        // Used to send integration events
        const Integration::OutboundIntegrationEventHandler& integrationEvents;

        // The maximum distance from the destination for which we can trigger a missed approach
        inline static const double MAX_DISTANCE_FROM_DESTINATION = 7.0;

        // The maximum altitude at which we can trigger a missed approach
        inline static const int MAX_ALTITUDE = 5000;

        // Minimum groundspeed at which we can trigger missed approach
        inline static const int MIN_GROUNDSPEED = 60;
    };
} // namespace UKControllerPlugin::MissedApproach
