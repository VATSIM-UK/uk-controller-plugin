#pragma once

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
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        };

    }  // namespace Hold
} // namespace UKControllerPlugin
