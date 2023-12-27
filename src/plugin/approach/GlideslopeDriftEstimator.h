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

    class GlideslopeDriftEstimator
    {
        public:
        [nodiscard] auto CalculateGlideslopeDrift(
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget, const Runway::Runway& runway) -> int;
    };
} // namespace UKControllerPlugin::Approach
