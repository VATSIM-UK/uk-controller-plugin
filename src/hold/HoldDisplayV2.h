#pragma once
#include "navaids/Navaid.h"

namespace UKControllerPlugin {
    namespace Hold {
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
        class HoldDisplayV2
        {
            public:
                HoldDisplayV2(
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Navaids::Navaid& navaid
                );
                void ButtonClicked(std::string button);
                INT GetDataStartHeight(void) const;
                POINT GetDisplayPos(void) const;
                Gdiplus::Rect GetTitleArea(void) const;
                RECT GetTitleClickArea(void) const;
                Gdiplus::Rect GetMinimiseArea(void) const;
                RECT GetMinimiseClickArea(void) const;
                Gdiplus::Rect GetInformationArea(void) const;
                RECT GetInformationClickArea(void) const;
                Gdiplus::Rect GetPlusArea(void) const;
                RECT GetPlusClickArea(void) const;
                Gdiplus::Rect GetMinusArea(void) const;
                RECT GetMinusClickArea(void) const;
                Gdiplus::Rect GetAllArea(void) const;
                RECT GetAllClickArea(void) const;
                Gdiplus::Rect GetAddArea(void) const;
                RECT GetAddClickArea(void) const;
                unsigned int GetLevelsSkipped(void) const;
                int GetWindowHeight(void) const;
                bool IsInInformationMode(void) const;
                bool IsMinimised(void) const;
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

                // Max levels skippable
                const unsigned int maxLevelsSkippable;

                // How high lines should be
                const INT lineHeight = 17;

                // The default offset for buttons
                const INT buttonStartOffset = 18;

                // The default offset for data
                const INT dataStartOffset = 68;

            private:

                void DrawRoundRectangle(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    const Gdiplus::Rect & rect,
                    UINT8 radius
                ) const;

                void RenderHoldInformation(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
                ) const;
                void RenderTitleBar(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
                ) const;
                void RenderManagedHoldDisplay(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
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
                Gdiplus::Rect titleArea = { 0, 0, 200, 15 };
                RECT titleRect = { 0, 0, 200, 15 };
                Gdiplus::Rect minimiseButtonArea = { 0, 0, 11, 11 };
                RECT minimiseClickRect;
                Gdiplus::Rect informationButtonArea = { 21, 0, 11, 11 };
                RECT informationClickRect;

                // Where to start the data drawing.
                INT dataStartHeight;

                // Is the window minimised
                bool minimised = false;

                // The window width
                int windowWidth = 200;

                // The window height
                int windowHeight = 500;

                // The maximum possible window height based on the number of levels in the hold
                int maxWindowHeight = 500;

                // The height of the window to use when doing the information display
                const int informationDisplayWindowHeight = 225;

                // How many levels to not draw
                unsigned int numLevelsSkipped = 0;

                // Should we display the information about the hold
                bool showHoldInformation = false;

                // Some more rects
                Gdiplus::Rect minusButtonRect = {5, this->buttonStartOffset, 40, 40};
                RECT minusButtonClickRect = { 5, this->buttonStartOffset, 40, 40 };
                Gdiplus::Rect plusButtonRect = {55, this->buttonStartOffset, 40, 40};
                RECT plusButtonClickRect = { 55, this->buttonStartOffset, 40, 40 };
                Gdiplus::Rect allButtonRect = { 105, this->buttonStartOffset, 40, 40 };
                RECT allButtonClickRect = { 105, this->buttonStartOffset, 40, 40 };
                Gdiplus::Rect addButtonRect = {190, this->buttonStartOffset, 40, 40};
                RECT addButtonClickRect = { 190, this->buttonStartOffset, 40, 40 };

                POINT windowPos = { 100, 100 };


                // The navaid that the hold is against
                const UKControllerPlugin::Navaids::Navaid& navaid;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
