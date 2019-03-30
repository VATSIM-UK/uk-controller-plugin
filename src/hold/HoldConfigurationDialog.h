#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"
#include "hold/HoldConfigurationMenuItem.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldWindowManager;
        class HoldSelectionMenu;
        class HoldProfileManager;
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
                    UKControllerPlugin::Hold::HoldSelectionMenu & holdSelectionMenu,
                    UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager
                );
                bool AddHold(UKControllerPlugin::Hold::HoldingData hold);
                size_t CountHolds(void) const;
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // The ES menu item that triggered this dialog - it should be notified when changes occur
                UKControllerPlugin::Hold::HoldConfigurationMenuItem * configurationItem;

                // Manages hold profiles
                UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager;

                // Hold selection menu
                UKControllerPlugin::Hold::HoldSelectionMenu & holdSelectionMenu;

                // Manages hold windows
                UKControllerPlugin::Hold::HoldWindowManager & windowManager;

                // The holds.
                std::set<
                    UKControllerPlugin::Hold::HoldingData,
                    UKControllerPlugin::Hold::CompareHolds
                > holds;


                // The holds that have been selected
                std::set<unsigned int> selectedHolds;

                // The hold profile currently selected
                unsigned int selectedHoldProfile = 0;
                int selectedHoldProfileIndex = 0;
        };

    }  // namespace Hold
} // namespace UKControllerPlugin
