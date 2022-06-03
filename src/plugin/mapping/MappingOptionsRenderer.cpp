#include "MappingOptionsRenderer.h"
#include "MappingRenderOptions.h"
#include "components/Checkbox.h"
#include "components/Title.h"
#include "components/TitleBar.h"
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
        if (objectDescription == DISPLAY_VRP_CHECKBOX) {
            mappingOptions->ShowVisualReferencePoints(!mappingOptions->ShowVisualReferencePoints());
            return;
        }
    }

    void MappingOptionsRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(50, 50, [&graphics, &radarScreen, this]() {
            const auto viewport = radarScreen.GetRadarViewport();
            // Background
            const auto width = viewport.right - viewport.left - 100;
            graphics.FillRect(Gdiplus::Rect{0, 0, width, viewport.bottom - viewport.top - 100}, *backgroundBrush);

            // Titlebar
            Components::TitleBar::Create(L"Mapping Options", Gdiplus::Rect{0, 0, width, 25})
                ->WithDefaultBackgroundBrush()
                ->WithDefaultTextBrush()
                ->Draw(graphics, radarScreen);

            // VRPs
            Components::Title::Create(L"Visual Reference Points", Gdiplus::Rect{10, 35, 300, 25})->Draw(graphics);
            Components::Checkbox::Create(L"Display VRPs", {10, 70}, screenObjectId, DISPLAY_VRP_CHECKBOX)
                ->Checked(mappingOptions->ShowVisualReferencePoints())
                ->Draw(graphics, radarScreen);
        });
    }
} // namespace UKControllerPlugin::Mapping
