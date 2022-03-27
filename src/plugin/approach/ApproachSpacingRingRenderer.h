#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;
    class ApproachSequencerDisplayOptions;
    class ApproachSpacingCalculator;

    /**
     * Draws rings around aircraft on approach based on
     * the distance we want between them and the next aircraft on approach.
     */
    class ApproachSpacingRingRenderer : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSpacingRingRenderer(
            ApproachSequencer& sequencer,
            ApproachSpacingCalculator& spacingCalculator,
            std::shared_ptr<ApproachSequencerDisplayOptions> options,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        // The sequencer
        ApproachSequencer& sequencer;

        // Calculates required spacings
        ApproachSpacingCalculator& spacingCalculator;

        // Display options
        std::shared_ptr<ApproachSequencerDisplayOptions> options;

        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Brush for drawing
        Gdiplus::SolidBrush circleBrush;
    };
} // namespace UKControllerPlugin::Approach
