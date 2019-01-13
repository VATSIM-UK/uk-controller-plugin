#pragma once
#include "intention/SectorExitPoint.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Class that handles three specific sector exit points into Shanwick -
            NIBOG, MIMKU, GOMUP.
        */
        class SectorExitPointShanwick : public SectorExitPoint
        {
            public:
                SectorExitPointShanwick(std::string name, std::string intentionCode, unsigned int outDirection)
                    : SectorExitPoint(name, intentionCode, outDirection) {}  // namespace IntentionCode
                std::string GetIntentionCode(
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route,
                    int foundPointIndex,
                    int cruiseLevel
                ) const override;
            private:
                bool SectorExitPointShanwick::IsViaBelfast(
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route,
                    int foundPointIndex
                ) const;
            };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
