#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "plugin/PopupMenuItem.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace MinStack {
        class MinStackManager;
    }  // namespace MinStack
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
    namespace MinStack {

        /*
            A class for rendering the Minimum Stack Levels
            to the display.
        */
        class MinStackRenderer :
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
                MinStackRenderer(
                    UKControllerPlugin::MinStack::MinStackManager & minStackModule,
                    int closeClickspotId,
                    int menuBarClickspotId,
                    int mslClickspotId,
                    int toggleCallbackFunctionId,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes
                );
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting);
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting);
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const;
                void Configure(int functionId, std::string subject, RECT screenObjectArea);
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
                const std::string visibleUserSettingKey = "DisplayMinStack";

                // The description when saving the visibility ASR setting
                const std::string visibleUserSettingDescription = "Minimum Stack Level Visibility";

                // The ASR key for the windows top left X coordinate
                const std::string xPositionUserSettingKey = "MinStackScreenPosX";

                // The description when saving the X position ASR setting
                const std::string xPositionUserSettingDescription = "Minimum Stack Level X Position";

                // The ASR key for the windows top left Y coordinate
                const std::string yPositionUserSettingKey = "MinStackScreenPosY";

                // The description when saving the Y position ASR setting
                const std::string yPositionUserSettingDescription = "Minimum Stack Level Y Position";

                // The MSL module menu text
                const std::string menuItemDescription = "Display Minimum Stack Levels";

                // The callback function ID for the toggle function
                const int toggleCallbackFunctionId;

            private:

                int RenderMinStacks(
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

                // Whether or not the window should be visible on screen.
                bool visible = false;

                // Brushes
                const UKControllerPlugin::Windows::GdiplusBrushes & brushes;

                // The module to render data for
                UKControllerPlugin::MinStack::MinStackManager & minStackModule;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
