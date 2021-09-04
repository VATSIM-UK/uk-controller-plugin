#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Controller {
    class ControllerPositionCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Prenote {

    class PrenoteMessage;
    class PrenoteMessageCollection;

    /*
     * Displays a list of departure release requests
     * for a given callsign
     */
    class PrenoteMessageStatusView : public RadarScreen::RadarRenderableInterface
    {
        public:
        explicit PrenoteMessageStatusView(
            std::shared_ptr<PrenoteMessageCollection> prenotes,
            const Controller::ControllerPositionCollection& controllers);
        static void DisplayFor(const std::string& callsign, std::chrono::seconds seconds);
        [[nodiscard]] static auto GetDisplayingFor() -> std::string;
        [[nodiscard]] static auto GetDisplayPosition() -> POINT;
        [[nodiscard]] static auto GetDisplayUntil() -> std::chrono::system_clock::time_point;
        static void MoveAllInstances(POINT position);
        static auto ShouldDisplay() -> bool;
        auto IsVisible() const -> bool override;
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            std::string objectDescription,
            POINT mousePos,
            RECT itemArea) override{};
        void RightClick(
            int objectId,
            std::string objectDescription,
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override{};
        void Move(RECT position, std::string objectDescription) override{};
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override{};

        private:
        [[nodiscard]] static auto GetTextColourForMessage(const std::shared_ptr<PrenoteMessage>& message) -> COLORREF;
        [[nodiscard]] auto GetControllerColumnEntryDimensions(int rowMultiplier) const -> RECT;
        [[nodiscard]] auto GetAcknowledgedColumnEntryDimensions(int rowMultiplier) const -> RECT;

        // The width of the menu
        const int baseWidth = 170;
        const int baseHeight = 20;
        const int heighPerPrenote = 25;
        const int textInsets = 5;

        // As this display shows for a brief period, this sets the display until time
        inline static std::chrono::system_clock::time_point displayUntil = // NOLINT
            (std::chrono::system_clock::time_point::min)();

        // This display moves around depending on where gets clicked
        inline static POINT displayPosition = {250, 250}; // NOLINT
        static const LONG CONTROLLER_COLUMN_WIDTH = 100;
        static const LONG GAP_BETWEEN_COLUMNS = 10;
        static const LONG ACKNOWLEDGED_COLUMN_WIDTH = 50;

        // Release event handler
        const std::shared_ptr<PrenoteMessageCollection> prenotes;

        // All the controllers
        const Controller::ControllerPositionCollection& controllers;

        // Who are we displaying this view for
        inline static std::string displayingFor = ""; // NOLINT

        // Brushes
        const Gdiplus::SolidBrush backgroundBrush;
        const Gdiplus::SolidBrush titleBarTextBrush;

        // Colours
        static const BYTE BACKGROUND_BRUSH_R_B = 58;
        static const BYTE BACKGROUND_BRUSH_G = 58;
        static const BYTE TITLE_BAR_TEXT_BRUSH = 227;
    };
} // namespace UKControllerPlugin::Prenote
