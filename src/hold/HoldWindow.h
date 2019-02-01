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
                void DisplayWindow(void);

            private:

                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                void RegisterWindowClass(void);

                // Handle to EuroScopes main window
                const HWND euroscopeWindow;

                // Handle to the DLL, needed to register the window to the module
                const HINSTANCE dllInstance;

                // Handle to the window instance
                HWND selfHandle;

                // The window class object
                WNDCLASSEX windowClass = { 0 };

                // Window class name
                const LPCWSTR windowClassName = L"HoldWindowClass";

                // Have we registered the window
                bool windowRegistered = false;
        };

        LRESULT CALLBACK WndProcTest(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    }  // namespace Hold
}  // namespace UKControllerPlugin
