#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
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

    namespace Releases {
        class DepartureReleaseRequest;
        class DepartureReleaseEventHandler;

        /*
         * Renders a list of departure releases requiring the users
         * decision.
         */
        class DepartureReleaseDecisionList : public RadarScreen::RadarRenderableInterface,
                                             public Euroscope::AsrEventHandlerInterface
        {
            public:
            DepartureReleaseDecisionList(
                DepartureReleaseEventHandler& handler,
                Euroscope::EuroscopePluginLoopbackInterface& plugin,
                const Controller::ControllerPositionCollection& controllers,
                int screenObjectId);
            void LeftClick(
                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                int objectId,
                std::string objectDescription,
                POINT mousePos,
                RECT itemArea) override;
            void RightClick(
                int objectId,
                std::string objectDescription,
                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
            auto IsVisible() const -> bool override;
            void Move(RECT position, std::string objectDescription) override;
            void Render(
                Windows::GdiGraphicsInterface& graphics,
                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
            void ResetPosition() override;
            void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
            void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
            void ToggleVisible();
            auto ContentCollapsed() const -> bool;
            auto Position() const -> Gdiplus::PointF;

            private:
            static auto GetAsrKey(const std::string& item) -> std::string;
            static auto GetAsrDescription(const std::string& description) -> std::string;

            // The controllers
            const Controller::ControllerPositionCollection& controllers;

            // Handles events for departure releases
            DepartureReleaseEventHandler& handler;

            // Provides interface with the plugin
            Euroscope::EuroscopePluginLoopbackInterface& plugin;

            // Drawing RECTs
            const Gdiplus::Rect callsignColumnHeader{5, 5, 100, 25};
            const Gdiplus::Rect controllerColumnHeader{115, 5, 100, 25};
            const Gdiplus::Rect airportColumnHeader{225, 5, 40, 25};
            const Gdiplus::Rect sidColumnHeader{275, 5, 65, 25};

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
            const int titleBarWidth = 340;

            // Default X/Y of window
            static const int defaultPosition = 100;

            // Default RECT on reset position
            inline static const RECT defaultRect = {100, 100, 200, 200};
            const Gdiplus::Rect closeButtonOffset = {325, 5, 10, 10};
            const Gdiplus::Rect collapseButtonOffset = {310, 5, 10, 10};

            // How high each line is
            static const int lineHeight = 25;

            // Our window position
            Gdiplus::PointF position = {0, 0};

            // Some colours
            const Gdiplus::Color OFF_WHITE_COLOUR = Gdiplus::Color(227, 227, 227);
            const Gdiplus::Color TITLE_BAR_BASE_COLOUR = Gdiplus::Color(197, 129, 214);
            const Gdiplus::Color TITLE_BAR_FLASH_COLOUR = Gdiplus::Color(255, 97, 93);

            // Standard window components
            std::shared_ptr<Components::TitleBar> titleBar;
            std::shared_ptr<Components::Button> closeButton;
            std::shared_ptr<Components::Button> collapseButton;
            std::shared_ptr<Components::BrushSwitcher> brushSwitcher;
        };

    } // namespace Releases
} // namespace UKControllerPlugin
