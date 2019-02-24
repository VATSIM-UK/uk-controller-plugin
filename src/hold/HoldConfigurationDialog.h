#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHoldsDescription.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldWindowManager;
        class HoldSelectionMenu;
    }  // namespace Hold
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that handles the hold configuration
            dialog which allows users to interact with hold
            profiles and select holds to display.
        */
        class HoldConfigurationDialog
        {
            public:
                HoldConfigurationDialog(
                    UKControllerPlugin::Hold::HoldWindowManager & windowManager,
                    UKControllerPlugin::Hold::HoldSelectionMenu & holdSelectionMenu
                );
                bool AddHold(UKControllerPlugin::Hold::HoldingData hold);
                size_t CountHolds(void) const;
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // Hold selection menu
                UKControllerPlugin::Hold::HoldSelectionMenu & holdSelectionMenu;

                // Manages hold windows
                UKControllerPlugin::Hold::HoldWindowManager & windowManager;

                // The holds.
                std::set<
                    UKControllerPlugin::Hold::HoldingData,
                    UKControllerPlugin::Hold::CompareHoldsDescription
                > holds;
        };

    }  // namespace Hold
} // namespace UKControllerPlugin
