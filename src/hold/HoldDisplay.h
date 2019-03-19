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
                HoldDisplay(const HoldDisplay & copy);
                ~HoldDisplay();
                void Redraw(void) const;

                // The hold this display is managing.
                const UKControllerPlugin::Hold::ManagedHold & managedHold;

            private:

                void DrawRoundRectangle(Gdiplus::Graphics* g, const Gdiplus::Pen *p, Gdiplus::Rect & rect, UINT8 radius);
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
                const Gdiplus::Font plusFont;
                Gdiplus::StringFormat stringFormat;

                // Titlebar
                const Gdiplus::RectF titleArea = { 0.0f, 0.0f, 235.0f, 15.0f };
                const Gdiplus::RectF exitButtonArea = { 247.0f, 2.0f, 11.0f, 11.0f };

                // How high lines should be
                const float lineHeight = 17.0f;

                // Where to start drawing data
                const float buttonStartHeight = this->titleArea.Y + this->titleArea.Height + 3;
                const float dataStartHeight = this->buttonStartHeight + 50;
                const int windowWidth = 235;
                const int windowHeight = 500;

                // Some more rects
                Gdiplus::Rect minusButtonRect = {5, (int)this->buttonStartHeight, 40, 40};
                Gdiplus::RectF minusButtonTextRect = {5.0f, this->buttonStartHeight, 40.0f, 40.0f};
                Gdiplus::Rect plusButtonRect = {55, (int)this->buttonStartHeight, 40, 40};
                Gdiplus::RectF plusButtonTextRect = {55.0f, this->buttonStartHeight, 40.0f, 40.0f};
                Gdiplus::Rect addButtonRect = {190, (int)this->buttonStartHeight, 40, 40};
                Gdiplus::RectF addButtonTextRect = {190.0f, this->buttonStartHeight, 40.0f, 40.0f};

                /*
                    How many levels to not draw
                */
                unsigned int numLevelsSkipped = 0;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
