#pragma once
#include "AirfieldGroup.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    // Pre Declarations
    class AirfieldGroup;
    // End

    /*
        Class representing Shannon (EINN), which has a special intention code.
    */
    class ShannonAirfieldGroup : public AirfieldGroup
    {
        public:
        ShannonAirfieldGroup();
        auto HasAirfield(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool override;
        auto GetIntentionCodeForGroup(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> std::string override;

        protected:
        bool Initialise() override;
    };
} // namespace UKControllerPlugin::IntentionCode
