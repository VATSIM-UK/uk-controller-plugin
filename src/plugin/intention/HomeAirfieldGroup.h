#pragma once
#include "AirfieldGroup.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /*
        Class representing all airfields within the United Kingdom (EGXX).
    */
    class HomeAirfieldGroup : public AirfieldGroup
    {
        public:
        HomeAirfieldGroup();
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
