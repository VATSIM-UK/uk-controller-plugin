#pragma once
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Mapping {
    class MappingRenderOptions;
    
    /**
     * Renders the mapping options to the screen
     */
    class MappingOptionsRenderer : public RadarScreen::RadarRenderableInterface
    {
        public:
        MappingOptionsRenderer(std::shared_ptr<MappingRenderOptions> mappingOptions, int screenObjectId);
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        
        // The options
        std::shared_ptr<MappingRenderOptions> mappingOptions;
        
        // For clickspots
        const int screenObjectId;
        
        // Brushes
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;
    };
} // namespace UKControllerPlugin::Mapping
