#pragma once
#include "hold/HoldConfigurationMenuItem.h"
#include "navaids/NavaidCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that handles the hold configuration
            dialog which allows users to select which holds
            they would like to display
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
                void SaveDialog(HWND hwnd);
                void InitDialog(HWND hwnd, LPARAM lParam);

                // The ES menu item that triggered this dialog - it should be notified when changes occur
                UKControllerPlugin::Hold::HoldConfigurationMenuItem * configurationItem = nullptr;

                // Contains all the navaids
                const UKControllerPlugin::Navaids::NavaidCollection& navaids;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
