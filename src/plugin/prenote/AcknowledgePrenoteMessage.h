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
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    /**
     * Provides a EuroScope TAG function for choosing
     * which prenote messages to cancel.
     */
    class AcknowledgePrenoteMessage
    {
        public:
        AcknowledgePrenoteMessage(
            std::shared_ptr<PrenoteMessageCollection> messages,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            TaskManager::TaskRunnerInterface& taskRunner,
            const Api::ApiInterface& api);
        void Acknowledge(Euroscope::EuroScopeCFlightPlanInterface& flightplan);

        private:
        // All active prenote messages
        std::shared_ptr<PrenoteMessageCollection> messages;

        // Who's active as a controller
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Runs async tasks
        TaskManager::TaskRunnerInterface& taskRunner;

        // Makes API calls
        const Api::ApiInterface& api;
    };
} // namespace UKControllerPlugin::Prenote
