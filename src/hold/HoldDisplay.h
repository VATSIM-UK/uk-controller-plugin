#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Manages the display of a single hold.
        */
        class HoldDisplay
        {
            public:
                HoldDisplay(HWND euroscopeWindow, HINSTANCE dllInstance);
                ~HoldDisplay();

            private:

                void PaintWindow(HDC hdc);
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // The window handle
                HWND selfHandle;

                void RegisterWindowClass(HINSTANCE dllInstance);

                // Window class name
                const LPCWSTR windowClassName = L"HoldDisplayClass";

                // The window class object
                WNDCLASSEX windowClass = { 0 };

                // Has the window class been registered with windows
                static bool windowRegistered;

                const Gdiplus::SolidBrush titleBarBrush;

                const HBRUSH backgroundBrush;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
