#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Components {
        class ClickableArea;
        class TitleBar;
    } // namespace Components
    namespace List {
        class PopupListInterface;
    } // namespace List
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerDisplayOptions;

    /**
     * Displays the approach sequencer.
     */
    class ApproachSequencerDisplay : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSequencerDisplay(
            ApproachSequencer& sequencer,
            std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
            std::shared_ptr<List::PopupListInterface> airfieldSelector,
            std::shared_ptr<List::PopupListInterface> callsignSelector,
            int screenObjectId);
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;

        private:
        void RenderBackground(
            Windows::GdiGraphicsInterface& graphics);
        void RenderAirfield(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void RenderDivider(Windows::GdiGraphicsInterface& graphics);
        void RenderHeaders(Windows::GdiGraphicsInterface& graphics);
        void RenderAddButton(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void
        RenderContent(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        // Dimensions
        inline static const int WINDOW_WIDTH = 435;
        inline static const int TITLE_BAR_HEIGHT = 20;
        inline static const int INSETS = 5;

        // Clickspot
        const std::string AIRFIELD_SELECTOR_CLICKSPOT = "aircraftSelector";
        const std::string ADD_AIRCRAFT_CLICKSPOT = "addAircraft";

        // The sequences
        ApproachSequencer& sequencer;

        // Display options
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions;

        // Selects an airfield
        std::shared_ptr<List::PopupListInterface> airfieldSelector;

        // Selects callsigns to add
        std::shared_ptr<List::PopupListInterface> callsignSelector;

        // Components
        const Gdiplus::Rect titleBarArea = {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldStaticArea = {INSETS, TITLE_BAR_HEIGHT + INSETS, 75, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldTextArea = {
            airfieldStaticArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 40, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect addButton = {
            airfieldTextArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 40, TITLE_BAR_HEIGHT};
        const Gdiplus::Point dividerLeft = {0, airfieldTextArea.GetBottom() + INSETS};
        const Gdiplus::Point dividerRight = {WINDOW_WIDTH, airfieldTextArea.GetBottom() + INSETS};
        const Gdiplus::Rect numberHeader = {INSETS, dividerLeft.Y + INSETS, 15, 25};
        const Gdiplus::Rect callsignHeader = {numberHeader.GetRight() + INSETS, numberHeader.GetTop(), 100, 25};
        const Gdiplus::Rect targetHeader = {callsignHeader.GetRight() + INSETS, callsignHeader.GetTop(), 75, 25};
        const Gdiplus::Rect actualHeader = {targetHeader.GetRight() + INSETS, targetHeader.GetTop(), 75, 25};
        const Gdiplus::Rect actionsHeader = {actualHeader.GetRight() + INSETS, actualHeader.GetTop(), 125, 25};

        std::shared_ptr<Components::TitleBar> titleBar;
        std::shared_ptr<Components::ClickableArea> airfieldClickspot;
        std::shared_ptr<Components::ClickableArea> addClickspot;

        const Gdiplus::Color BACKGROUND_COLOUR = Gdiplus::Color(64, 64, 64);
        const Gdiplus::Color TEXT_COLOUR = Gdiplus::Color(225, 225, 225);
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;
        std::shared_ptr<Gdiplus::Brush> textBrush;
        std::shared_ptr<Gdiplus::Pen> dividingPen;
    };
} // namespace UKControllerPlugin::Approach
