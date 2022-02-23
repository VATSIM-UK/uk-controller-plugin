#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Approach {
    /**
     * Draws rings around aircraft on approach based on
     * the distance we want between them and the next aircraft on approach.
     */
    class ApproachSpacingRingRenderer : public RadarScreen::RadarRenderableInterface
    {
        public:
        ApproachSpacingRingRenderer(Euroscope::EuroscopePluginLoopbackInterface& plugin);
        auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Brush for drawing
        Gdiplus::SolidBrush circleBrush;
    };
} // namespace UKControllerPlugin::Approach
