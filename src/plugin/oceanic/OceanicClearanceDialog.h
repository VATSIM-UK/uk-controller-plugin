#pragma once

namespace UKControllerPlugin {
    namespace Oceanic {

        /*
            Displays the oceanic clearance for the specified callsign
        */
        class OceanicClearanceDialog
        {
            public:
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void InitDialog(HWND hwnd, LPARAM lParam);

                static std::wstring ConvertItemForDialog(std::string item);
                static void SetDialogItem(HWND hwnd, int itemId, std::wstring value);
        };
    } // namespace Oceanic
}  // namespace UKControllerPlugin
