#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "navaids/NavaidCollection.h"

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
                    const UKControllerPlugin::Navaids::NavaidCollection& navaids
                );
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
                UKControllerPlugin::Hold::HoldConfigurationMenuItem * configurationItem = nullptr;

                // Contains all the navaids
                const UKControllerPlugin::Navaids::NavaidCollection& navaids;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
