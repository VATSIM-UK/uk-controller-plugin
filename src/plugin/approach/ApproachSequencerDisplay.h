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
    class ApproachSequencerDisplayOptions;

    /**
     * Displays the approach sequencer.
     */
    class ApproachSequencerDisplay : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSequencerDisplay(
            std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
            std::shared_ptr<List::PopupListInterface> airfieldSelector,
            int screenObjectId);
        auto IsVisible() const -> bool override;
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
        void RenderAirfield(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        // Dimensions
        inline static const int WINDOW_WIDTH = 435;
        inline static const int TITLE_BAR_HEIGHT = 20;
        inline static const int CONTENT_HEIGHT = 150;
        inline static const int INSETS = 5;

        // Clickspot
        const std::string AIRFIELD_SELECTOR_CLICKSPOT = "aircraftSelector";

        // Display options
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions;

        // Selects an airfield
        std::shared_ptr<List::PopupListInterface> airfieldSelector;

        Gdiplus::Rect titleBarArea = {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
        Gdiplus::Rect contentArea = {0, TITLE_BAR_HEIGHT, WINDOW_WIDTH, CONTENT_HEIGHT};
        Gdiplus::Rect airfieldStaticArea = {INSETS, TITLE_BAR_HEIGHT + INSETS, 75, TITLE_BAR_HEIGHT};
        Gdiplus::Rect airfieldTextArea = {
            airfieldStaticArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 40, TITLE_BAR_HEIGHT};

        std::shared_ptr<Components::TitleBar> titleBar;
        std::shared_ptr<Components::ClickableArea> airfieldClickspot;

        const Gdiplus::Color BACKGROUND_COLOUR = Gdiplus::Color(64, 64, 64);
        const Gdiplus::Color TEXT_COLOUR = Gdiplus::Color(225, 225, 225);
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;
        std::shared_ptr<Gdiplus::Brush> textBrush;
    };
} // namespace UKControllerPlugin::Approach
