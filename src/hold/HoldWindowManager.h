#pragma once
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldManager;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that encapsulates the procedures surrounding
            the hold manager window.
        */
        class HoldWindowManager
        {
            public:

                HoldWindowManager(
                    HWND euroscopeWindow,
                    HINSTANCE dllInstance,
                    const UKControllerPlugin::Hold::HoldManager & holdManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );
                void AddWindow(void);
                size_t CountDisplays(void) const;
                void RefreshWindows(void) const;

            private:

                // The id that has been assigned as a the callback id when a menu item is clicked
                const unsigned int itemClickedFunctionId = 666;

                // The plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The hold manager
                const UKControllerPlugin::Hold::HoldManager & holdManager;

                // Handle to EuroScopes main window
                const HWND euroscopeWindow;

                // Handle to the DLL, needed to register the window to the module
                const HINSTANCE dllInstance;

                // Set of holds to display
                std::list<UKControllerPlugin::Hold::HoldDisplay> holdDisplays;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
