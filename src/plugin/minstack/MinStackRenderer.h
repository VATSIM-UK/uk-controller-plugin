#pragma once
#include "dialog/DialogManager.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "minstack/MinStackRendererConfiguration.h"
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
// END

namespace UKControllerPlugin::MinStack {
    class MinStackManager;

    /*
        A class for rendering the Minimum Stack Levels
        to the display.
    */
    class MinStackRenderer : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface,
                             public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
                             public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
    {
        public:
        MinStackRenderer(
            UKControllerPlugin::MinStack::MinStackManager& minStackModule,
            int closeClickspotId,
            int menuBarClickspotId,
            int mslClickspotId,
            int toggleCallbackFunctionId,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
            const UKControllerPlugin::Dialog::DialogManager& dialogManager);
        void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;
        void Configure(
            [[maybe_unused]] int functionId,
            [[maybe_unused]] std::string subject,
            [[maybe_unused]] RECT screenObjectArea) override;
        auto GetConfig() -> UKControllerPlugin::MinStack::MinStackRendererConfiguration&;
        [[nodiscard]] auto GetHideClickspotArea() const -> RECT;
        [[nodiscard]] auto GetHideSpotRender() const -> Gdiplus::Rect;
        [[nodiscard]] auto GetTopBarArea() const -> RECT;
        [[nodiscard]] auto GetTopBarRender() const -> Gdiplus::Rect;
        void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            [[maybe_unused]] POINT mousePos,
            [[maybe_unused]] RECT itemArea) override;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Move(RECT titleBarArea, [[maybe_unused]] std::string objectDescription) override;
        void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void RightClick(
            int objectId,
            const std::string& objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        void SetVisible(bool visible);

        [[nodiscard]] auto VisibleUserSettingKey() const -> const std::string&;
        [[nodiscard]] auto SelectedMinStackUserSettingKey() const -> const std::string&;
        [[nodiscard]] auto SelectedMinStackUserSettingDescription() const -> const std::string&;
        [[nodiscard]] auto VisibleUserSettingDescription() const -> const std::string&;
        [[nodiscard]] auto XPositionUserSettingKey() const -> const std::string&;
        [[nodiscard]] auto XPositionUserSettingDescription() const -> const std::string&;
        [[nodiscard]] auto YPositionUserSettingKey() const -> const std::string&;
        [[nodiscard]] auto YPositionUserSettingDescription() const -> const std::string&;
        [[nodiscard]] auto MenuItemDescription() const -> const std::string&;
        [[nodiscard]] auto HideClickspotId() const -> int;
        [[nodiscard]] auto MslClickspotId() const -> int;
        [[nodiscard]] auto LeftColumnWidth() const -> int;
        [[nodiscard]] auto HideClickspotWidth() const -> int;
        [[nodiscard]] auto RowHeight() const -> int;

        private:
        auto RenderMinStacks(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const -> int;
        void RenderOuterFrame(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int numMinStacks);
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
        UKControllerPlugin::MinStack::MinStackRendererConfiguration config;

        // The module to render data for
        UKControllerPlugin::MinStack::MinStackManager& minStackModule;

        // Spawns the configuration dialog
        const UKControllerPlugin::Dialog::DialogManager& dialogManager;

        // The EuroScope ID for the close button.
        const int hideClickspotId;

        // The clickspot id for the menu bar
        const int menuBarClickspotId;

        // The clickspot id for each MSL
        const int mslClickspotId;

        // Width of the left column
        const int leftColumnWidth = 75;

        // Width of the right column
        const int rowHeight = 15;

        // Width of the hide clickspot
        const int hideClickspotWidth = 25;

        // The ASR key for whether or not to display the module
        const std::string visibleUserSettingKey = "DisplayMinStack";

        // The ASR key for saving the selected the selected minimum stack levels
        const std::string selectedMinStackUserSettingKey = "SelectedMinStack";

        // The description when saving the selected minimum stack levels
        const std::string selectedMinStackUserSettingDescription = "Selected Minimum Stack Levels To Display";

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
        const std::string menuItemDescription = "Configure Minimum Stack Levels";

        // The callback function ID for the toggle function
        const int toggleCallbackFunctionId;

        static const int DEFAULT_POSITION = 100;
    };
} // namespace UKControllerPlugin::MinStack
