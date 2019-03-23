#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {
        class ManagedHold;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
    }  // namespace Windows
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
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const UKControllerPlugin::Hold::ManagedHold & managedHold
                );
                HoldDisplay(const HoldDisplay & copy);
                void ButtonClicked(std::string button);
                void Move(const POINT & pos);
                void PaintWindow(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
                ) const;

                // The hold this display is managing.
                const UKControllerPlugin::Hold::ManagedHold & managedHold;

            private:

                void DrawRoundRectangle(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    const Gdiplus::Rect & rect,
                    UINT8 radius
                ) const;

                // The window handle
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // Brushes
                const Gdiplus::SolidBrush titleBarTextBrush;
                const Gdiplus::SolidBrush titleBarBrush;
                const Gdiplus::SolidBrush dataBrush;
                const Gdiplus::SolidBrush clearedLevelBrush;
                const Gdiplus::HatchBrush blockedLevelBrush;
                const Gdiplus::Pen borderPen;
                const Gdiplus::SolidBrush exitButtonBrush;
                const Gdiplus::SolidBrush backgroundBrush;

                // Fonts
                const Gdiplus::FontFamily fontFamily;
                const Gdiplus::Font font;
                const Gdiplus::Font plusFont;
                Gdiplus::StringFormat stringFormat;

                // Titlebar
                Gdiplus::Rect titleArea = { 0, 0, 235, 15 };
                RECT titleRect = { 0, 0, 235, 15 };
                Gdiplus::RectF exitButtonArea = { 247.0f, 2.0f, 11.0f, 11.0f };

                // How high lines should be
                const INT lineHeight = 17;

                // Where to start drawing data
                const INT buttonStartHeight = this->titleArea.Y + this->titleArea.Height + 3;
                const INT dataStartHeight = this->buttonStartHeight + 50;
                const int windowWidth = 235;
                const int windowHeight = 500;

                // Some more rects
                Gdiplus::Rect minusButtonRect = {5, (int)this->buttonStartHeight, 40, 40};
                RECT minusButtonClickRect = { 5, (int)this->buttonStartHeight, 40, 40 };
                Gdiplus::Rect plusButtonRect = {55, (int)this->buttonStartHeight, 40, 40};
                RECT plusButtonClickRect = { 5, (int)this->buttonStartHeight, 40, 40 };
                Gdiplus::Rect addButtonRect = {190, (int)this->buttonStartHeight, 40, 40};
                RECT addButtonClickRect = { 5, (int)this->buttonStartHeight, 40, 40 };

                POINT windowPos = { 0, 0 };

                /*
                    How many levels to not draw
                */
                unsigned int numLevelsSkipped = 0;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
