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
     * Provides a EuroScope TAG function for choosing
     * which prenote messages to cancel.
     */
    class CancelPrenoteMessageMenu
    {
        public:
        explicit CancelPrenoteMessageMenu(
            std::shared_ptr<PrenoteMessageCollection> messages,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            TaskManager::TaskRunnerInterface& taskRunner,
            const Api::ApiInterface& api,
            int callbackId);
        void DisplayPrenoteToDeleteMenu(Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos);
        void ControllerForPrenoteDeletionSelected(const std::string& callsign);

        private:
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
    };
} // namespace UKControllerPlugin::Prenote
