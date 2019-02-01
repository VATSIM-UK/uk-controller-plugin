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
                HWND CreateHoldView(HWND hwnd, unsigned int x, unsigned int y);
                HWND CreateButton(HWND hwnd);
                HWND CreateStatic(HWND hwnd);
                void CreateHoldInformation(HWND parent, unsigned int x, unsigned int y);

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

                // A prop for the hold window object so we can get at it
                static const LPCWSTR objectProp;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
