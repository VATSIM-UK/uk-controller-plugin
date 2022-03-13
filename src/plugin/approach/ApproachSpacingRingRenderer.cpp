#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSpacingRingRenderer.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "geometry/DistanceRadiusToScreenRadius.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Approach {

    ApproachSpacingRingRenderer::ApproachSpacingRingRenderer(
        ApproachSequencer& sequencer,
        std::shared_ptr<ApproachSequencerDisplayOptions> options,
        Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : sequencer(sequencer), options(std::move(options)), plugin(plugin),
          circleBrush(Gdiplus::SolidBrush(Gdiplus::Color(50, 255, 255, 255)))
    {
    }

    bool ApproachSpacingRingRenderer::IsVisible() const
    {
        return true;
    }

    void ApproachSpacingRingRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        if (options->Airfield().empty() || !sequencer.GetForAirfield(options->Airfield()).First()) {
            return;
        }

        auto aircraft = sequencer.GetForAirfield(options->Airfield()).First();
        while (aircraft != nullptr) {
            const auto flightplan = plugin.GetFlightplanForCallsign(aircraft->Callsign());
            const auto radarTarget = plugin.GetRadarTargetForCallsign(aircraft->Callsign());
            if (!flightplan || !radarTarget) {
                aircraft = aircraft->Next();
                return;
            }

            double circleRadius = Geometry::ScreenRadiusFromDistance(6, radarScreen);
            auto position = radarScreen.ConvertCoordinateToScreenPoint(radarTarget->GetPosition());

            graphics.FillCircle(
                {static_cast<Gdiplus::REAL>(position.x - circleRadius),
                 static_cast<Gdiplus::REAL>(position.y - circleRadius),
                 static_cast<Gdiplus::REAL>(circleRadius * 2),
                 static_cast<Gdiplus::REAL>(circleRadius * 2)},
                circleBrush);
            aircraft = aircraft->Next();
        }
    }
} // namespace UKControllerPlugin::Approach
