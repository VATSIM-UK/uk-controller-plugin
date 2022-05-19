#pragma once
#include "SectorExitPoint.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /*
        Special class to handle the LELNA SectorExitPoint.
    */
    class SectorExitPointLelna : public SectorExitPoint
    {
        public:
        SectorExitPointLelna(std::string name, std::string intentionCode, unsigned int outDirection)
            : SectorExitPoint(name, intentionCode, outDirection)
        {
        } // namespace IntentionCode
        std::string GetIntentionCode(
            UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route,
            int foundPointIndex,
            int cruiseLevel) const override;
    };
} // namespace UKControllerPlugin::IntentionCode
