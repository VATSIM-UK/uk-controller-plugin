#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Components {
        class ClickableArea;
        class TitleBar;
    } // namespace Components
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace List {
        class PopupListInterface;
    } // namespace List
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerOptions;
    class ApproachSequencerDisplayOptions;
    class ApproachSpacingCalculator;

    /**
     * Displays the approach sequencer.
     */
    class ApproachSequencerDisplay : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSequencerDisplay(
            ApproachSequencer& sequencer,
            ApproachSpacingCalculator& spacingCalculator,
            ApproachSequencerOptions& options,
            std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
            std::shared_ptr<List::PopupListInterface> airfieldSelector,
            std::shared_ptr<List::PopupListInterface> callsignSelector,
            std::shared_ptr<List::PopupListInterface> targetSelector,
            std::shared_ptr<List::PopupListInterface> airfieldTargetSelector,
            std::shared_ptr<List::PopupListInterface> airfieldSeparationSelector,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
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
        void RenderBackground(Windows::GdiGraphicsInterface& graphics);
        void RenderAirfield(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void RenderAirfieldTarget(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void RenderAirfieldSeparation(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void RenderDivider(Windows::GdiGraphicsInterface& graphics);
        void RenderHeaders(Windows::GdiGraphicsInterface& graphics);
        void RenderAddButton(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);
        void
        RenderContent(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        // Dimensions
        inline static const int WINDOW_WIDTH = 435;
        inline static const int TITLE_BAR_HEIGHT = 15;
        inline static const int INSETS = 5;

        // Clickspot
        const std::string AIRFIELD_SELECTOR_CLICKSPOT = "aircraftSelector";
        const std::string ADD_AIRCRAFT_CLICKSPOT = "addAircraft";
        const std::string AIRFIELD_TARGET_CLICKSPOT = "airfieldTarget";
        const std::string AIRFIELD_SEPARATION_CLICKSPOT = "airfieldSeparation";

        // The sequences
        ApproachSequencer& sequencer;

        // The calculator for the spacing
        ApproachSpacingCalculator& spacingCalculator;

        // General options
        ApproachSequencerOptions& options;

        // Display options
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions;

        // Selects an airfield
        std::shared_ptr<List::PopupListInterface> airfieldSelector;

        // Selects callsigns to add
        std::shared_ptr<List::PopupListInterface> callsignSelector;

        // Selecting the target
        std::shared_ptr<List::PopupListInterface> targetSelector;

        // For selecting the default airfield target
        std::shared_ptr<List::PopupListInterface> airfieldTargetSelector;

        // For selecting an airfields separation requirement
        std::shared_ptr<List::PopupListInterface> airfieldSeparationSelector;

        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
        
        // Pens and brushes
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes;

        // The screen object id
        int screenObjectId;

        // Components
        const Gdiplus::Rect titleBarArea = {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldStaticArea = {INSETS, TITLE_BAR_HEIGHT + INSETS, 75, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldTextArea = {
            airfieldStaticArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 40, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect addButton = {
            airfieldTextArea.GetRight() + INSETS, airfieldStaticArea.GetTop(), 110, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldTargetStatic = {
            INSETS, airfieldStaticArea.GetBottom() + INSETS, 75, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldTargetTextArea = {
            airfieldTargetStatic.GetRight() + INSETS, airfieldTargetStatic.GetTop(), 75, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldSeparationStatic = {
            airfieldTargetTextArea.GetRight() + INSETS, airfieldTargetTextArea.GetTop(), 90, TITLE_BAR_HEIGHT};
        const Gdiplus::Rect airfieldSeparationTextArea = {
            airfieldSeparationStatic.GetRight() + INSETS, airfieldSeparationStatic.GetTop(), 75, TITLE_BAR_HEIGHT};
        const Gdiplus::Point dividerLeft = {0, airfieldTargetStatic.GetBottom() + INSETS};
        const Gdiplus::Point dividerRight = {WINDOW_WIDTH, airfieldTargetStatic.GetBottom() + INSETS};
        const Gdiplus::Rect numberHeader = {INSETS, dividerLeft.Y + INSETS, 15, 25};
        const Gdiplus::Rect callsignHeader = {numberHeader.GetRight() + INSETS, numberHeader.GetTop(), 100, 25};
        const Gdiplus::Rect targetHeader = {callsignHeader.GetRight() + INSETS, callsignHeader.GetTop(), 75, 25};
        const Gdiplus::Rect actualHeader = {targetHeader.GetRight() + INSETS, targetHeader.GetTop(), 75, 25};
        const Gdiplus::Rect actionsHeader = {actualHeader.GetRight() + INSETS, actualHeader.GetTop(), 125, 25};

        std::shared_ptr<Components::TitleBar> titleBar;
        std::shared_ptr<Components::ClickableArea> airfieldClickspot;
        std::shared_ptr<Components::ClickableArea> addClickspot;
        std::shared_ptr<Components::ClickableArea> airfieldTargetClickspot;
        std::shared_ptr<Components::ClickableArea> airfieldSeparationClickspot;

    };
} // namespace UKControllerPlugin::Approach
