#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"

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
                HoldConfigurationDialog();
                bool AddHold(UKControllerPlugin::Hold::HoldingData hold);
                size_t CountHolds(void) const;
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // The holds.
                std::set<UKControllerPlugin::Hold::HoldingData, UKControllerPlugin::Hold::CompareHolds> holds;
        };

    }  // namespace Hold
} // namespace UKControllerPlugin
