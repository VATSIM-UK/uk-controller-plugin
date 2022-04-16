#pragma once
#include "intention/SectorExitPoint.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /*
        Special case SectorExitPoint for ETRAT.
    */
    class SectorExitPointEtrat : public SectorExitPoint
    {
        public:
        SectorExitPointEtrat(std::string name, std::string intentionCode, unsigned int outDirection)
            : SectorExitPoint(name, intentionCode, outDirection)
        {
        }
        std::string GetIntentionCode(
            UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route,
            int foundPointIndex,
            int cruiseLevel) const override;

        private:
        // Special destinations get the E3 code
        const std::set<std::string> destinations{"LFRG", "LFRK", "LFOH", "LFOE"};
    };
} // namespace UKControllerPlugin::IntentionCode
