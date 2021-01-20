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
            bool AppliesToController(
                std::string callsign,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route
            ) const override;
            bool HasAirfield(
                std::string airfield,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
            ) const;
            std::string GetIntentionCodeForGroup(
                std::string airfield,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
            ) const;

            const std::string PRIMARY_CODE = "EB";
            const std::string SECONDARY_CODE = "EX";

        protected:
            bool Initialise(void);

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
            std::set<std::string> secondaryAirfields = {
                "EBKT",
                "EBOS"
            };
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
