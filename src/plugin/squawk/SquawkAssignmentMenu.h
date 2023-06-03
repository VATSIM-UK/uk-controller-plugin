#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Squawk {

    class SquawkGeneratorInterface;

    /**
     * A popup menu for choosing a squawk code to assign to a flight.
     */
    class SquawkAssignmentMenu
    {
        public:
        SquawkAssignmentMenu(
            int callbackId,
            int manualSquawkCallbackId,
            SquawkGeneratorInterface& squawkGenerator,
            Controller::ActiveCallsignCollection& activeCallsigns,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void DisplaySquawkAssignmentMenu(Euroscope::EuroScopeCFlightPlanInterface& flightplan, const POINT& mousePos);
        void MenuOptionSelected(const std::string& option, const RECT& tagItemArea);
        void ManualSquawkEntered(const std::string& squawk);

        private:
        void AddMenuOptions();

        // Menu options
        const char* GENERAL_SQUAWK = "General";
        const char* LOCAL_SQUAWK = "Local";
        const char* CONSPICUITY = "Conspicuity (7000)";
        const char* CIRCUIT = "Circuit (7010)";
        const char* MANUAL = "Manual";

        std::list<const char*> squawkOptions = {GENERAL_SQUAWK, LOCAL_SQUAWK, CONSPICUITY, CIRCUIT, MANUAL};

        // Menu sizes
        static const int MENU_WIDTH = 200;
        static const int MENU_HEIGHT = 200;

        // The callback id to use for when a menu item is clicked
        const int callbackId;

        // The callback id to use for when the manual squawk menu item is clicked and a squawk provided
        const int manualSquawkCallbackId;

        // The squawk generator
        SquawkGeneratorInterface& squawkGenerator;

        // Who's active as a controller
        Controller::ActiveCallsignCollection& activeCallsigns;

        // The plugin instance, lets us make menus
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };

} // namespace UKControllerPlugin::Squawk
