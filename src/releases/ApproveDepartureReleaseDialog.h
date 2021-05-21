#pragma once

namespace UKControllerPlugin {

    namespace Releases {
        /*
        *  Dialog for approving departure releases
        */
        class ApproveDepartureReleaseDialog
        {
            public:
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                void InitDialog(HWND hwnd, LPARAM lParam);
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        };
    } // namespace Releases
}  // namespace UKControllerPlugin
