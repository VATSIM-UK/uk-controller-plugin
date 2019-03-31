#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {
        class ManagedHold;
        class HoldManager;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroscopeRadarLoopbackInterface;
        class UserSetting;
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
                    const UKControllerPlugin::Hold::ManagedHold & managedHold,
                    UKControllerPlugin::Hold::HoldManager & holdManager
                );
                void ButtonClicked(std::string button);
                POINT GetDisplayPos(void) const;
                unsigned int GetLevelsSkipped(void) const;
                void LoadDataFromAsr(
                    UKControllerPlugin::Euroscope::UserSetting & userSetting,
                    unsigned int profileId
                );
                void Move(const POINT & pos);
                void PaintWindow(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
                ) const;
                void SaveDataToAsr(
                    UKControllerPlugin::Euroscope::UserSetting & userSetting,
                    unsigned int profileId,
                    std::string profileName
                ) const;

                // The hold this display is managing.
                const UKControllerPlugin::Hold::ManagedHold & managedHold;

            private:

                void DrawRoundRectangle(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    const Gdiplus::Rect & rect,
                    UINT8 radius
                ) const;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // Reference to the plugin
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

                // The default offset for buttons
                const INT buttonStartOffset = 18;

                // The default offset for data
                const INT dataStartOffset = 68;

                // Where to start the data drawing.
                INT dataStartHeight;


                int windowWidth = 235;
                int windowHeight = 500;

                // Some more rects
                Gdiplus::Rect minusButtonRect = {5, (int)this->buttonStartOffset, 40, 40};
                RECT minusButtonClickRect = { 5, (int)this->buttonStartOffset, 40, 40 };
                Gdiplus::Rect plusButtonRect = {55, (int)this->buttonStartOffset, 40, 40};
                RECT plusButtonClickRect = { 5, (int)this->buttonStartOffset, 40, 40 };
                Gdiplus::Rect addButtonRect = {190, (int)this->buttonStartOffset, 40, 40};
                RECT addButtonClickRect = { 5, (int)this->buttonStartOffset, 40, 40 };

                POINT windowPos = { 100, 100 };

                /*
                    How many levels to not draw
                */
                unsigned int numLevelsSkipped = 0;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
