#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
    } // namespace Controller

    namespace Components {
        class BrushSwitcher;
        class TitleBar;
        class Button;
    } // namespace Components

    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class AcknowledgePrenoteMessage;
    class PrenoteMessageCollection;
    class PrenoteMessage;

    /*
     * Renders a list of departure releases requiring the users
     * decision.
     */
    class PendingPrenoteList : public RadarScreen::RadarRenderableInterface, public Euroscope::AsrEventHandlerInterface
    {
        public:
        PendingPrenoteList(
            std::shared_ptr<PrenoteMessageCollection> messages,
            std::shared_ptr<AcknowledgePrenoteMessage> acknowledger,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            int screenObjectId);
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        auto IsVisible() const -> bool override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        void ToggleVisible();
        auto ContentCollapsed() const -> bool;
        auto Position() const -> Gdiplus::PointF;

        private:
        [[nodiscard]] static auto GetAsrKey(const std::string& item) -> std::string;
        [[nodiscard]] static auto GetAsrDescription(const std::string& description) -> std::string;

        // The controllers
        const Controller::ControllerPositionCollection& controllers;

        // All the prenote messages
        const std::shared_ptr<PrenoteMessageCollection> messages;

        // Lets us acknowledge messages
        std::shared_ptr<AcknowledgePrenoteMessage> acknowledger;

        // Provides interface with the plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Drawing RECTs
        const Gdiplus::Rect callsignColumnHeader{5, 5, 100, 25};
        const Gdiplus::Rect controllerColumnHeader{115, 5, 100, 25};
        const Gdiplus::Rect airportColumnHeader{225, 5, 40, 25};
        const Gdiplus::Rect sidColumnHeader{275, 5, 65, 25};
        const Gdiplus::Rect destColumnHeader{350, 5, 40, 25};

        const Gdiplus::Color OFF_WHITE_COLOUR = Gdiplus::Color(255, 255, 255);
        const Gdiplus::Color TITLE_BAR_BASE_COLOUR = Gdiplus::Color(130, 50, 154);
        const Gdiplus::Color TITLE_BAR_FLASH_COLOUR = Gdiplus::Color(179, 3, 0);

        // Brushes
        const Gdiplus::SolidBrush textBrush;

        // Clickspot identifier
        const int screenObjectId;

        // Is the window visible
        bool visible;

        // Is the content collapsed
        bool contentCollapsed;

        // Height of title bar
        const int titleBarHeight = 20;

        // Width of title bar
        const int titleBarWidth = 390;

        // Height of each line
        const int LINE_HEIGHT = 25;

        // Our window position
        Gdiplus::PointF position = {0, 0};

        inline static const RECT DEFAULT_POSITION = {100, 100, 200, 200};
        const Gdiplus::Rect CLOSE_BUTTON_OFFSET = {375, 5, 10, 10};
        const Gdiplus::Rect COLLAPSE_BUTTON_OFFSET = {360, 5, 10, 10};

        // Standard window components
        std::shared_ptr<Components::TitleBar> titleBar;
        std::shared_ptr<Components::Button> closeButton;
        std::shared_ptr<Components::Button> collapseButton;
        std::shared_ptr<Components::BrushSwitcher> brushSwitcher;
    };
} // namespace UKControllerPlugin::Prenote
