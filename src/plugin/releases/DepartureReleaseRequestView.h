#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Controller {
    class ControllerPositionCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Releases {

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
            const DepartureReleaseEventHandler& handler, const Controller::ControllerPositionCollection& controllers);
        static void DisplayFor(std::chrono::seconds seconds);
        static void MoveAllInstances(POINT position);
        static auto ShouldDisplay() -> bool;
        static auto GetCoordinates() -> POINT;
        auto IsVisible() const -> bool override;
        void Move(RECT position, std::string objectDescription) override{};
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override{};

        private:
        static auto GetControllerColour(const std::shared_ptr<DepartureReleaseRequest>& request) -> COLORREF;
        static auto GetTimerColour(const std::shared_ptr<DepartureReleaseRequest>& request) -> COLORREF;
        static auto GetControllerColumnEntryDimensions(int rowMultiplier) -> RECT;
        static auto GetReleasedColumnEntryDimensions(int rowMultiplier) -> RECT;
        static auto GetExpiredColumnEntryDimensions(int rowMultiplier) -> RECT;

        // The width of the menu
        static const int BASE_WIDTH = 230;
        static const int BASE_HEIGHT = 20;
        static const int HEIGHT_PER_RELEASE = 25;
        static const int TEXT_INSETS = 5;
        static const int CONTROLLER_COLUMN_WIDTH = 100;
        static const int GAP_BETWEEN_COLUMNS = 10;
        static const int RELEASED_COLUMN_WIDTH = 50;
        static const int EXPIRED_COLUMN_WIDTH = 40;

        // As this display shows for a brief period, this sets the display until time
        inline static std::chrono::system_clock::time_point displayUntil = // NOLINT
            (std::chrono::system_clock::time_point::min)();

        // This display moves around depending on where gets clicked
        inline static POINT displayPosition = {250, 250}; // NOLINT

        // Release event handler
        const DepartureReleaseEventHandler& handler;

        // All the controllers
        const Controller::ControllerPositionCollection& controllers;

        // Brushes
        const Gdiplus::SolidBrush backgroundBrush;
        const Gdiplus::SolidBrush titleBarTextBrush;

        // Colours
        const Gdiplus::Color textColour = Gdiplus::Color(227, 227, 227);
        const Gdiplus::Color backgroundColour = Gdiplus::Color(58, 57, 58);
    };
} // namespace UKControllerPlugin::Releases
