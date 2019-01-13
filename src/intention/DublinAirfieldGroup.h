#pragma once
#include "intention/AirfieldGroup.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Class representing airfields in and underneath the Dublin TMA such that any arrivals
            would be considered "Dublin TMA Landers".
        */
        class DublinAirfieldGroup : public UKControllerPlugin::IntentionCode::AirfieldGroup
        {
            public:
                DublinAirfieldGroup(void);
                bool HasAirfield(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const;
                std::string GetIntentionCodeForGroup(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const;
            protected:
                bool Initialise(void);
            };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
