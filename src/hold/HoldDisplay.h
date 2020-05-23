#pragma once
#include "navaids/Navaid.h"
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"
#include "hold/CompareHoldingAircraft.h"
#include "dialog/DialogManager.h"

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
        class HoldDisplay
        {
            public:
                HoldDisplay(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Navaids::Navaid& navaid,
                    const std::set<
                        UKControllerPlugin::Hold::HoldingData,
                        UKControllerPlugin::Hold::CompareHolds
                    >& publishedHolds,
                    const UKControllerPlugin::Dialog::DialogManager& dialogManager
                );
                void ButtonClicked(std::string button);
                void CallsignClicked(
                    std::string callsign,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                    POINT mousePos,
                    RECT area
                );
                void ClearedLevelClicked(
                    std::string callsign,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                    POINT mousePos,
                    RECT area
                );
                Gdiplus::Rect GetHoldViewBackgroundRender(
                    const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>& aircraft
                ) const;
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
                int GetMaximumLevel(void) const;
                int GetMinimumLevel(void) const;
                void SetMaximumLevel(int level);
                void SetMinimumLevel(int level);
                std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>
                    MapAircraftToLevels(const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& aircraft) const;
                bool IsInInformationMode(void) const;
                bool IsMinimised(void) const;
                void LoadDataFromAsr(
                    UKControllerPlugin::Euroscope::UserSetting & userSetting
                );
                void Move(const POINT & pos);
                void PaintWindow(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    const int screenObjectId
                ) const;
                void SaveDataToAsr(
                    UKControllerPlugin::Euroscope::UserSetting & userSetting
                ) const;

                // The navaid that the hold is against
                const UKControllerPlugin::Navaids::Navaid& navaid;

                // How high lines should be
                const INT lineHeight = 25;

                // The default offset for buttons
                const INT buttonStartOffset = 18;

                // The default offset for data
                const INT dataStartOffset = 68;

                // The distance from the navaid at which the same level box renders
                const double sameLevelBoxDistance = 12.0;

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
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // For opening dialogs
                const UKControllerPlugin::Dialog::DialogManager& dialogManager;

                // The holds that are published for this display
                const std::set<
                    UKControllerPlugin::Hold::HoldingData,
                    UKControllerPlugin::Hold::CompareHolds
                >& publishedHolds;

                // Brushes
                const Gdiplus::SolidBrush titleBarTextBrush;
                const Gdiplus::SolidBrush titleBarBrush;
                const Gdiplus::SolidBrush dataBrush;
                const Gdiplus::SolidBrush clearedLevelBrush;
                const Gdiplus::SolidBrush blockedLevelBrush;
                const Gdiplus::Pen borderPen;
                const Gdiplus::Pen sameLevelBoxPen;
                Gdiplus::Pen verticalSpeedAscentPen;
                Gdiplus::Pen verticalSpeedDescentPen;
                const Gdiplus::SolidBrush exitButtonBrush;
                const Gdiplus::SolidBrush backgroundBrush;

                // Fonts
                const Gdiplus::FontFamily fontFamily;
                const Gdiplus::Font font;
                const Gdiplus::Font plusFont;
                Gdiplus::StringFormat stringFormat;

                // Where to start the data drawing.
                INT dataStartHeight;

                // Is the window minimised
                bool minimised = false;

                // The window width
                int windowWidth = 225;

                // The height of the window to use when doing the information display
                const int informationDisplayWindowHeight = 225;

                // The minimum level in the hold
                int minimumLevel = 7000;

                // The maximum level in the hold
                int maximumLevel = 15000;

                // Should we display the information about the hold
                bool showHoldInformation = false;

                // Titlebar
                Gdiplus::Rect titleArea = { 0, 0, this->windowWidth, 15 };
                RECT titleRect = { 0, 0, this->windowWidth, 15 };
                Gdiplus::Rect minimiseButtonArea = { 0, 0, 11, 11 };
                RECT minimiseClickRect;
                Gdiplus::Rect informationButtonArea = { 21, 0, 11, 11 };
                RECT informationClickRect;
                Gdiplus::Rect optionsButtonArea = { 42, 0, 11, 11 };
                RECT optionsClickRect;

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
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
