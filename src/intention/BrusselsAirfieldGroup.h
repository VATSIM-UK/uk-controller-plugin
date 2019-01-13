#pragma once
#include "intention/AirfieldGroup.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        // Predeclarations
        class AirfieldGroup;
        // End

        /*
            Class representing the Brussels (EBXX) airfield group.
        */
        class BrusselsAirfieldGroup : public AirfieldGroup
        {
        public:
            BrusselsAirfieldGroup(void);
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
