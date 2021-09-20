#pragma once
#include "AirfieldGroup.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    /*
        Class representing airfields in and underneath the Dublin TMA such that any arrivals
        would be considered "Dublin TMA Landers".
    */
    class DublinAirfieldGroup : public UKControllerPlugin::IntentionCode::AirfieldGroup
    {
        public:
        DublinAirfieldGroup();
        auto HasAirfield(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool override;
        auto GetIntentionCodeForGroup(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> std::string override;

        protected:
        auto Initialise() -> bool override;
    };
} // namespace UKControllerPlugin::IntentionCode
