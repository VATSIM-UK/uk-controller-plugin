#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that encapsulates the procedures surrounding
            the hold manager window.
        */
        class HoldWindow
        {
            public:

                HoldWindow(HWND euroscopeWindow, HINSTANCE dllInstance);


            private:

                LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // Handle to EuroScopes main window
                const HWND euroscopeWindow;

                // Handle to the DLL, needed to register the window to the module
                const HINSTANCE dllInstance;

                // Handle to the window instance
                HWND selfHandle;

                // The window class object
                WNDCLASSEX windowClass;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
