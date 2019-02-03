#pragma once
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that encapsulates the procedures surrounding
            the hold manager window.
        */
        class HoldWindowManager
        {
            public:

                HoldWindowManager(HWND euroscopeWindow, HINSTANCE dllInstance);
                void AddWindow(void);



            private:
                // Handle to EuroScopes main window
                const HWND euroscopeWindow;

                // Handle to the DLL, needed to register the window to the module
                const HINSTANCE dllInstance;

                // Set of holds to display
                std::list<UKControllerPlugin::Hold::HoldDisplay> holdDisplays;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
