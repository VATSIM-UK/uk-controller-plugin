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
    struct GlideslopeDrift
    {
        int drift;
        double perpendicularDistanceFromLocaliser;
        double localiserRange;
    };

    class GlideslopeDriftEstimator
    {
        public:
        [nodiscard] auto CalculateGlideslopeDrift(
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget, const Runway::Runway& runway) const
            -> GlideslopeDrift;
    };
} // namespace UKControllerPlugin::Approach
