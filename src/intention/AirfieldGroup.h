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
                virtual bool Initialise(void) = 0;
                bool IsViaPoint(
                    std::string point,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface
                    & route
                ) const;
            private:
                std::vector <std::string> airfields;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
