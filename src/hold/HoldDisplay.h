#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {
        class ManagedHold;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Manages the display of a single hold.
        */
        class HoldDisplay
        {
            public:
                HoldDisplay(
                    HWND euroscopeWindow,
                    HINSTANCE dllInstance,
                    const UKControllerPlugin::Hold::ManagedHold & managedHold
                );
                ~HoldDisplay();

            private:

                void PaintWindow(HDC hdc);
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // The hold this display is managing.
                const UKControllerPlugin::Hold::ManagedHold & managedHold;

                // The window handle
                HWND selfHandle;

                void RegisterWindowClass(HINSTANCE dllInstance);

                // Window class name
                const LPCWSTR windowClassName = L"HoldDisplayClass";

                // The window class object
                WNDCLASSEX windowClass = { 0 };

                // Has the window class been registered with windows
                static bool windowRegistered;

                // Brushes
                const Gdiplus::SolidBrush titleBarTextBrush;
                const Gdiplus::SolidBrush titleBarBrush;
                const Gdiplus::SolidBrush dataBrush;
                const Gdiplus::SolidBrush clearedLevelBrush;
                const Gdiplus::HatchBrush blockedLevelBrush;
                const Gdiplus::Pen borderPen;
                const Gdiplus::SolidBrush exitButtonBrush;
                const HBRUSH backgroundBrush;

                // Fonts
                const Gdiplus::FontFamily fontFamily;
                const Gdiplus::Font font;
                Gdiplus::StringFormat stringFormat;

                // Titlebar
                const Gdiplus::RectF titleArea = { 0.0f, 0.0f, 225.0f, 15.0f };
                const Gdiplus::RectF exitButtonArea = { 212.0f, 2.0f, 11.0f, 11.0f };

                // How high lines should be
                const float lineHeight = 17.0f;

                // Where to start drawing data
                const float dataStartHeight = this->titleArea.Y + this->titleArea.Height + 3;
                const int windowWidth = 225;
                const int windowHeight = 250;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
