#pragma once
#include "intention/SectorExitPoint.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /*
        Special case SectorExitPoint for ETRAT.
    */
    class SectorExitPointKonan : public SectorExitPoint
    {
        public:
        SectorExitPointKonan(std::string name, std::string intentionCode, unsigned int outDirection)
            : SectorExitPoint(name, intentionCode, outDirection)
        {
        } // namespace IntentionCode
        std::string GetIntentionCode(
            UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route,
            int foundPointIndex,
            int cruiseLevel) const override;
    };
} // namespace UKControllerPlugin::IntentionCode
