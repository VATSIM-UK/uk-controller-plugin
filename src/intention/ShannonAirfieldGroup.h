#pragma once
#include "intention/AirfieldGroup.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        // Pre Declarations
        class AirfieldGroup;
        // End

        /*
            Class representing Shannon (EINN), which has a special intention code.
        */
        class ShannonAirfieldGroup : public AirfieldGroup
        {
            public:
                ShannonAirfieldGroup(void);
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
