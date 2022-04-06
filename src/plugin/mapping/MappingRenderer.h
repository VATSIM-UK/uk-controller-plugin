#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Mapping {
    class MappingElementManager;

    /**
     * Renders everything mapping.
     */
    class MappingRenderer : public RadarScreen::RadarRenderableInterface
    {
        public:
        MappingRenderer(const MappingElementManager& elementManager);
        auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        // All the mapping elements
        const MappingElementManager& elementManager;
    };
} // namespace UKControllerPlugin::Mapping
