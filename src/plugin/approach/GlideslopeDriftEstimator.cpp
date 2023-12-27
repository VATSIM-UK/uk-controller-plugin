#include "GlideslopeDriftEstimator.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "runway/Runway.h"

namespace UKControllerPlugin::Approach {

    auto GlideslopeDriftEstimator::CalculateGlideslopeDrift(
        const Euroscope::EuroScopeCRadarTargetInterface& radarTarget, const Runway::Runway& runway) -> int
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

        // Calculate the difference between the glideslope altitude and the aircraft altitude
        return runway.GlideslopeAltitudeAtDistance(distance) - radarTarget.GetAltitude();
    }
} // namespace UKControllerPlugin::Approach
