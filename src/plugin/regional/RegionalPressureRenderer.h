#pragma once
#include "RegionalPressureRendererConfiguration.h"
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "plugin/PopupMenuItem.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Regional {

    class RegionalPressureManager;

    /*
        A class for rendering the minimum stack levels to display
    */
    class RegionalPressureRenderer : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
                                     public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
                                     public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
    {
        public:
        RegionalPressureRenderer(
            UKControllerPlugin::Regional::RegionalPressureManager& manager,
            int closeClickspotId,
            int menuBarClickspotId,
            int rpsClickspotId,
            int toggleCallbackFunctionId,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
            const UKControllerPlugin::Dialog::DialogManager& dialogManager);
        void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        auto GetConfig() -> UKControllerPlugin::Regional::RegionalPressureRendererConfiguration&;
        [[nodiscard]] auto GetHideClickspotArea() const -> RECT;
        [[nodiscard]] auto GetHideSpotRender() const -> Gdiplus::Rect;
        [[nodiscard]] auto GetTopBarArea() const -> RECT;
        [[nodiscard]] auto GetTopBarRender() const -> Gdiplus::Rect;
        void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Move(RECT titleBarArea, std::string objectDescription) override;
        void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void RightClick(
            int objectId,
            const std::string& objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        void SetVisible(bool visible);

        private:
        auto RenderPressures(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) -> int;
        void RenderOuterFrame(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int numRegionalPressures);
        void RenderTopBar(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        // The top bar rectangle
        RECT topBarArea = {};

        // The toggle spot rectangle
        RECT hideClickspotArea = {};

        // The rectangle to render for the top bar
        Gdiplus::Rect topBarRender;

        // The rectangle to render for the hide clickspot
        Gdiplus::Rect hideSpotRender;

        // Brushes
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes;

        // The configuration for the renderer
        UKControllerPlugin::Regional::RegionalPressureRendererConfiguration config;

        // The module to render data for
        UKControllerPlugin::Regional::RegionalPressureManager& manager;

        // Spawns the configuration dialog
        const UKControllerPlugin::Dialog::DialogManager& dialogManager;

        // The EuroScope ID for the close button.
        const int hideClickspotId;

        // The clickspot id for the menu bar
        const int menuBarClickspotId;

        // The clickspot id for each MSL
        const int rpsClickspotId;

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

        inline static const int DEFAULT_POSITION = 100;
        inline static const int LEFT_COLUMN_WIDTH = 100;
        inline static const int ROW_HEIGHT = 15;
        inline static const int HIDE_CLICKSPOT_WIDTH = 50;
        inline static const int APPEND_ZERO_LIMIT = 1000;
    };
} // namespace UKControllerPlugin::Regional
