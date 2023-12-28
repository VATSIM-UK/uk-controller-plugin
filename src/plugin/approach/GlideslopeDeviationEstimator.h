#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace Runway {
        class Runway;
    } // namespace Runway
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {
    // Struct representing glideslope deviation
    struct GlideslopeDeviation
    {
        int deviation;
        double perpendicularDistanceFromLocaliser;
        double localiserRange;
    };

    class GlideslopeDeviationEstimator
    {
        public:
        [nodiscard] auto CalculateGlideslopeDeviation(
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const Runway::Runway& runway) const -> GlideslopeDeviation;
    };
} // namespace UKControllerPlugin::Approach
