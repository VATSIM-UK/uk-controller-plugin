#include "GlideslopeDeviationEstimator.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "runway/Runway.h"

namespace UKControllerPlugin::Approach {

    auto GlideslopeDeviationEstimator::CalculateGlideslopeDeviation(
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget, const Runway::Runway& runway) const
        -> GlideslopeDeviation
    {
        // Calculate the slope of each line
        const auto runwaySlope = runway.RunwayHeadingLineSlope();
        const auto runwayPerpendicularSlope = runway.RunwayPerpendicularHeadingLineSlope();

        // Calculate the distance between the intersection and the threshold
        EuroScopePlugIn::CPosition intersection;
        intersection.m_Latitude = (runwaySlope * runway.Threshold().m_Latitude -
                                   runwayPerpendicularSlope * radarTarget.GetPosition().m_Latitude +
                                   radarTarget.GetPosition().m_Longitude - runway.Threshold().m_Longitude) /
                                  (runwaySlope - runwayPerpendicularSlope);
        intersection.m_Longitude =
            runwaySlope * (intersection.m_Latitude - runway.Threshold().m_Latitude) + runway.Threshold().m_Longitude;
        const auto distance = runway.Threshold().DistanceTo(intersection);

        return {
            .deviation = radarTarget.GetAltitude() - runway.GlideslopeAltitudeAtDistance(distance),
            .perpendicularDistanceFromLocaliser = radarTarget.GetPosition().DistanceTo(intersection),
            .localiserRange = distance};
    }
} // namespace UKControllerPlugin::Approach
