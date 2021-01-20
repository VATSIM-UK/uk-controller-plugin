#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"
#include "hold/HoldDisplay.h"
#include "navaids/NavaidCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that handles the hold configuration
            dialog which allows users to interact with hold
            profiles and select holds to display.
        */
        class HoldDisplayConfigurationDialog
        {
            public:
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // Min and max levels
                const int minHoldingLevel = 1000;
                const int maxHoldingLevel = 66000;

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void SaveDialog(HWND hwnd);
                void InitDialog(HWND hwnd, LPARAM lParam);
                bool ValueInRange(int value, int min, int max) const;

                // The hold display that triggered this dialog, needs to be updated when changes occur
                UKControllerPlugin::Hold::HoldDisplay* configurationItem = nullptr;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
