#pragma once

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A class for handling the dialog that configures the countdown timer.
        */
        class TimerConfigurationDialog
        {
            public:
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin
