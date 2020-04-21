#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {
        /*
            Abstract class that represents a group of airfields. Internally, it contains a list
            of all airfields within the group, as well as functions to check whether an airfield is
            known and to return an intention code for the group.
        */
        class AirfieldGroup {
            public:
                virtual bool AppliesToController(
                    std::string callsign,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route
                ) const;
                virtual bool HasAirfield(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const = 0;
                virtual std::string GetIntentionCodeForGroup(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const = 0;
            protected:
                void AddAirfieldToList(std::string airfield);
                bool AirfieldInList(std::string airfield) const;
                bool ControllerIsScottish(std::string callsign) const;
                virtual bool Initialise(void) = 0;
                bool IsViaPoint(
                    std::string point,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface
                    & route
                ) const;
            private:
                std::vector <std::string> airfields;

                const std::string SCAC_CALLSIGN_PREFIX = "SCO";
                const std::string SCTC_CALLSIGN_PREFIX = "STC";
                const std::string EGPX_CALLSIGN_PREFIX = "EGP";
                const std::string EGPX_MIL_CALLSIGN_PREFIX = "EGQ";
                const std::string EGPX_NI_CALLSIGN_PREFIX = "EGA";
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
