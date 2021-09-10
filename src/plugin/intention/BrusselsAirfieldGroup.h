#pragma once
#include "intention/AirfieldGroup.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {

    // Predeclarations
    class AirfieldGroup;
    // End

    /*
        Class representing the Brussels (EBXX) airfield group.
    */
    class BrusselsAirfieldGroup : public AirfieldGroup
    {
        public:
        BrusselsAirfieldGroup();
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
        const std::string PRIMARY_CODE = "EB";
        const std::string SECONDARY_CODE = "EX";

        // Airfields that should get the primary intention code
        std::set<std::string> primaryAirfields = {
            "EBBR",
            "EBCI",
            "EBAW",
            "EBMB",
            "EBCV",
            "EBLG",
        };

        // Airfields that should get the secondary intention code
        std::set<std::string> secondaryAirfields = {"EBKT", "EBOS"};
    };
} // namespace UKControllerPlugin::IntentionCode
