#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Controller {
        class ControllerPositionCollection;
    }
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
            Controller::ControllerPositionCollection& controllers,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void DisplayPrenoteToDeleteMenu(Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos);

        private:
        // All active prenote messages
        std::shared_ptr<PrenoteMessageCollection> messages;

        // All the controller positions
        const Controller::ControllerPositionCollection& controllers;

        // The plugin instance, lets us make menus
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
        
        static const int MENU_WIDTH = 400;
        static const int MENU_HEIGHT = 600;
    };
} // namespace UKControllerPlugin::Prenote
