#include "MappingOptionsRenderer.h"
#include "MappingRenderOptions.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Mapping {

    MappingOptionsRenderer::MappingOptionsRenderer(
        std::shared_ptr<MappingRenderOptions> mappingOptions, int screenObjectId)
        : mappingOptions(mappingOptions), screenObjectId(screenObjectId),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(170, 64, 64, 64)))
    {
        int i = this->screenObjectId == 1 ? 1 : 2;
        i++;
        assert(mappingOptions && "Mapping render options not set");
    }

    auto MappingOptionsRenderer::IsVisible() const -> bool
    {
        return mappingOptions->ShowOptions();
    }

    void MappingOptionsRenderer::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        RadarRenderableInterface::LeftClick(radarScreen, objectId, objectDescription, mousePos, itemArea);
    }

    void MappingOptionsRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto viewport = radarScreen.GetRadarViewport();
        graphics.FillRect(
            Gdiplus::Rect{50, 50, viewport.right - viewport.left - 100, viewport.bottom - viewport.top - 100},
            *backgroundBrush);
    }
} // namespace UKControllerPlugin::Mapping
