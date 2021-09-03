#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    /**
     * Provides a EuroScope TAG function for chosing a controller
     * to send a prenote to.
     */
    class SendPrenoteMenu
    {
        public:
        explicit SendPrenoteMenu(
            std::shared_ptr<PrenoteMessageCollection> messages,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            TaskManager::TaskRunnerInterface& taskRunner,
            const Api::ApiInterface& api,
            int callbackId);
        void
        DisplayControllerSelectionMenu(Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos);
        void ControllerForPrenoteSelected(std::string callsign);

        private:
        [[nodiscard]] auto PrenoteMessageExists(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan, int sendingControllerId, int targetControllerId) const
            -> bool;
        [[nodiscard]] static auto ResponseValid(nlohmann::json& response) -> bool;

        // All active prenote messages
        std::shared_ptr<PrenoteMessageCollection> messages;

        // All the controller positions
        const Controller::ControllerPositionCollection& controllers;

        // Who's active as a controller
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // The plugin instance, lets us make menus
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Runs async tasks
        TaskManager::TaskRunnerInterface& taskRunner;

        // Makes API calls
        const Api::ApiInterface& api;

        // The callback id to use for when a menu item is clicked
        const int callbackId;

        static const int MENU_WIDTH = 400;
        static const int MENU_HEIGHT = 600;
        static const int MESSAGE_EXPIRY_SECONDS = 300;
    };
} // namespace UKControllerPlugin::Prenote
