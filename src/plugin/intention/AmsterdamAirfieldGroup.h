#pragma once
#include "AirfieldGroup.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    // Predeclarations
    class AirfieldGroup;
    // End

    /*
        Class representing the Amsterdam (EHXX) airfield group.
    */
    class AmsterdamAirfieldGroup : public AirfieldGroup
    {
        public:
        AmsterdamAirfieldGroup();
        auto AppliesToController(
            const std::string& callsign, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool override;
        auto HasAirfield(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> bool override;
        auto GetIntentionCodeForGroup(
            const std::string& airfield, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> std::string override;

        protected:
        auto Initialise() -> bool override;

        private:
        const std::string CODE_VIA_KOKSY = "AS";
        const std::string CODE_NOT_VIA_KOKSY = "AM";
    };
} // namespace UKControllerPlugin::IntentionCode
