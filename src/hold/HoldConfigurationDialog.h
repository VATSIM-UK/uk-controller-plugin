#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"
#include "hold/HoldConfigurationMenuItem.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

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
                    UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager
                );
                bool AddHold(UKControllerPlugin::Hold::HoldingData hold);
                size_t CountHolds(void) const;
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void AddHoldToSelectedList(HWND hwnd);
                void CreateNewHoldProfile(HWND hwnd);
                void DeleteHoldProfile(HWND hwnd);
                void DestroyDialog(HWND hwnd);
                void DisplaySelectedProfile();
                void InitDialog(HWND hwnd, LPARAM lParam);
                void RemoveHoldFromSelectedList(HWND hwnd);
                void SelectProfile(HWND hwnd, WPARAM wParam);
                void UpdateHoldProfile(HWND hwnd);

                // The ES menu item that triggered this dialog - it should be notified when changes occur
                UKControllerPlugin::Hold::HoldConfigurationMenuItem * configurationItem;

                // Manages hold profiles
                UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager;

                // The holds.
                std::set<
                    UKControllerPlugin::Hold::HoldingData,
                    UKControllerPlugin::Hold::CompareHolds
                > holds;

                // The holds that have been selected
                std::set<unsigned int> selectedHolds;

                // The hold profile currently selected
                unsigned int selectedHoldProfile = 0;
                int selectedHoldProfileIndex = LB_ERR;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
