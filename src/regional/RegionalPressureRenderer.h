#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "plugin/PopupMenuItem.h"
#include "regional/RegionalPressureRendererConfiguration.h"
#include "dialog/DialogManager.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Regional {
        class RegionalPressureManager;
    }  // namespace Regional
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct  GdiplusBrushes;
    }  // namespace Windows
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Regional {

        /*
            A class for rendering the minimum stack levels to display
        */
        class RegionalPressureRenderer :
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
                RegionalPressureRenderer(
                    UKControllerPlugin::Regional::RegionalPressureManager & manager,
                    int closeClickspotId,
                    int menuBarClickspotId,
                    int mslClickspotId,
                    int toggleCallbackFunctionId,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager
                );
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting);
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting);
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const;
                void Configure(int functionId, std::string subject, RECT screenObjectArea);
                UKControllerPlugin::Regional::RegionalPressureRendererConfiguration & GetConfig(void);
                RECT GetHideClickspotArea(void) const;
                Gdiplus::Rect GetHideSpotRender(void) const;
                RECT GetTopBarArea(void) const;
                Gdiplus::Rect GetTopBarRender(void) const;
                void LeftClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                bool IsVisible(void) const;
                void Move(RECT titleBarArea, std::string objectDescription);
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                void ResetPosition(void) override;
                void SetVisible(bool visible);

                // The EuroScope ID for the close button.
                const int hideClickspotId;

                // The clickspot id for the menu bar
                const int menuBarClickspotId;

                // The clickspot id for each MSL
                const int mslClickspotId;

                // Width of the left column
                const int leftColumnWidth;

                // Width of the right column
                const int rowHeight;

                // Width of the hide clickspot
                const int hideClickspotWidth;

                // The ASR key for whether or not to display the module
                const std::string visibleUserSettingKey = "DisplayRegionalPressures";

                // The ASR key for saving the selected the selected minimum stack levels
                const std::string selectedRegionalPressureUserSettingKey = "SelectedRegionalPressures";

                // The description when saving the selected minimum stack levels
                const std::string selecteRegionalPressureUserSettingDescription = "Selected Regional Pressures To Display";

                // The description when saving the visibility ASR setting
                const std::string visibleUserSettingDescription = "Regional Pressure Visibility";

                // The ASR key for the windows top left X coordinate
                const std::string xPositionUserSettingKey = "RegionalPressureScreenPosX";

                // The description when saving the X position ASR setting
                const std::string xPositionUserSettingDescription = "Regional Pressure X Position";

                // The ASR key for the windows top left Y coordinate
                const std::string yPositionUserSettingKey = "RegionalPressureScreenPosY";

                // The description when saving the Y position ASR setting
                const std::string yPositionUserSettingDescription = "Regional Pressure Y Position";

                // The rps module menu text
                const std::string menuItemDescription = "Configure Regional Pressures";

                // The callback function ID for the toggle function
                const int toggleCallbackFunctionId;

            private:

                int RenderPressures(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );
                void RenderOuterFrame(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen,
                    int numMinStacks
                );
                void RenderTopBar(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                );

                // The top bar rectangle
                RECT topBarArea;

                // The toggle spot rectangle
                RECT hideClickspotArea;

                // The rectangle to render for the top bar
                Gdiplus::Rect topBarRender;

                // The rectangle to render for the hide clickspot
                Gdiplus::Rect hideSpotRender;

                // Brushes
                const UKControllerPlugin::Windows::GdiplusBrushes & brushes;

                // The configuration for the renderer
                UKControllerPlugin::Regional::RegionalPressureRendererConfiguration config;

                // The module to render data for
                UKControllerPlugin::Regional::RegionalPressureManager & manager;

                // Spawns the configuration dialog
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
