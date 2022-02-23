#include "ApproachSpacingRingRenderer.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "geometry/DistanceRadiusToScreenRadius.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Approach {

    ApproachSpacingRingRenderer::ApproachSpacingRingRenderer(Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : plugin(plugin), circleBrush(Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255)))
    {
    }

    bool ApproachSpacingRingRenderer::IsVisible() const
    {
        return true;
    }

    void ApproachSpacingRingRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        plugin.ApplyFunctionToAllFlightplans(
            [&graphics, &radarScreen, this](
                const std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface>& flightplan,
                const std::shared_ptr<Euroscope::EuroScopeCRadarTargetInterface>& radarTarget) {
                double circleRadius = Geometry::ScreenRadiusFromDistance(6, radarScreen);
                auto position = radarScreen.ConvertCoordinateToScreenPoint(radarTarget->GetPosition());
                
                graphics.FillCircle(
                    {static_cast<Gdiplus::REAL>(position.x - circleRadius),
                     static_cast<Gdiplus::REAL>(position.y - circleRadius),
                     static_cast<Gdiplus::REAL>(circleRadius * 2),
                     static_cast<Gdiplus::REAL>(circleRadius * 2)},
                    circleBrush);
            });
    }
} // namespace UKControllerPlugin::Approach
