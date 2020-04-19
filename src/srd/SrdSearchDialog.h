#pragma once

namespace UKControllerPlugin {
    namespace Srd {

        /*
            A class for performing SRD searches
        */
        class SrdSearchDialog
        {
            public:
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void InitDialog(HWND hwnd, LPARAM lParam);
        };
    }  // namespace Srd
}  // UKControllerPlugin
