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
    } // namespace Euroscope
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

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
            const Controller::ActiveCallsignCollection& activeCallsigns);
        void Trigger(Euroscope::EuroScopeCFlightPlanInterface& flightplan);

        private:
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
        const Controller::ActiveCallsignCollection& activeCallsigns;
    };
} // namespace UKControllerPlugin::MissedApproach
