#pragma once
#include "radarscreen/RadarRenderableInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"

namespace UKControllerPlugin {

    namespace Components {
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
                    int screenObjectId
                );
                ~DepartureReleaseDecisionList() override = default;
                void LeftClick(Euroscope::EuroscopeRadarLoopbackInterface& radarScreen, int objectId,
                               std::string objectDescription, POINT mousePos, RECT itemArea) override;
                void RightClick(int objectId, std::string objectDescription,
                                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
                bool IsVisible() const override;
                void Move(RECT position, std::string objectDescription) override;
                void Render(Windows::GdiGraphicsInterface& graphics,
                            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
                void ResetPosition() override;
                void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
                void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
                void ToggleVisible();

                // Handles events for departure releases
                DepartureReleaseEventHandler& handler;

                // Provides interface with the plugin
                Euroscope::EuroscopePluginLoopbackInterface& plugin;

            private:
                static std::string GetAsrKey(std::string item);
                static std::string GetAsrDescription(std::string description);

                // The requests requiring decision
                std::set<std::shared_ptr<const DepartureReleaseRequest>> requestsRequiringDecision;

                // Drawing RECTs
                const Gdiplus::Rect callsignColumnHeader{5, 5, 100, 30};
                const Gdiplus::Rect airportColumnHeader{115, 5, 40, 30};
                const Gdiplus::Rect sidColumnHeader{165, 5, 50, 30};

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
                const int titleBarWidth = 400;

                // Our window position
                Gdiplus::PointF position = {0, 0};

                // Standard window components
                std::shared_ptr<Components::TitleBar> titleBar;
                std::shared_ptr<Components::Button> closeButton;
                std::shared_ptr<Components::Button> collapseButton;
        };

    } // namespace Releases
} // namespace UKControllerPlugin
