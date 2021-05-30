#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {

    namespace Controller {
        class ControllerPositionCollection;
    } // namespace Controller

    namespace Releases {

        class DepartureReleaseEventHandler;
        class DepartureReleaseRequest;

        /*
         * Displays a list of departure release requests
         * for a given callsign
         */
        class DepartureReleaseRequestView : public RadarScreen::RadarRenderableInterface
        {
            public:
                explicit DepartureReleaseRequestView(
                    const DepartureReleaseEventHandler& handler,
                    const Controller::ControllerPositionCollection& controllers,
                    int screenObjectId
                );
                static void DisplayFor(std::chrono::seconds seconds);
                static void MoveAllInstances(POINT position);
                static bool ShouldDisplay();
                static POINT GetCoordinates();
                bool IsVisible() const override;
                void LeftClick(Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                               int objectId, std::string objectDescription, POINT mousePos, RECT itemArea) override {};
                void RightClick(int objectId, std::string objectDescription,
                                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override {};
                void Move(RECT position, std::string objectDescription) override {};
                void Render(Windows::GdiGraphicsInterface& graphics,
                            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
                void ResetPosition() override {};

            private:

                COLORREF GetControllerColour(std::shared_ptr<DepartureReleaseRequest> request);
                COLORREF GetTimerColour(std::shared_ptr<DepartureReleaseRequest> request);

                // The width of the menu
                const int baseWidth = 230;
                const int baseHeight = 20;
                const int heightPerRelease = 25;
                const int textInsets = 5;

                // As this display shows for a brief period, this sets the display until time
                inline static std::chrono::system_clock::time_point displayUntil =
                    (std::chrono::system_clock::time_point::min)();

                // This display moves around depending on where gets clicked
                inline static POINT displayPosition = {250, 250};

                // Release event handler
                const DepartureReleaseEventHandler& handler;

                // All the controllers
                const Controller::ControllerPositionCollection& controllers;
                
                const int screenObjectId;

                // Brushes
                const Gdiplus::SolidBrush backgroundBrush;
                const Gdiplus::SolidBrush titleBarTextBrush;
        };
    } // namespace Releases
} // namespace UKControllerPlugin
