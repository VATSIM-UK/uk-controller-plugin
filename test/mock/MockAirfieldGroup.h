#pragma once
#include "intention/AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPluginTest {
    namespace IntentionCode {
        class MockAirfieldGroup : public UKControllerPlugin::IntentionCode::AirfieldGroup
        {
            public:
                MockAirfieldGroup(bool applicableToController = true)
                {
                    this->applicableToController = applicableToController;
                }

                bool AppliesToController(
                    std::string callsign,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route
                ) const
                {
                    return this->applicableToController;
                }

                bool HasAirfield(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const {
                    return airfield == "EGLL" || airfield == "EGKK" || airfield == "EGNX";
                }

                std::string GetIntentionCodeForGroup(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const {
                    if (airfield == "EGLL") {
                        return "LL";
                    }

                    if (airfield == "EGKK") {
                        return "KK";
                    }

                    if (airfield == "EGNX") {
                        return "NX";
                    }

                    return "NOPE";
                }

            protected:
                bool Initialise(void)
                {
                    return true;
                }

                bool applicableToController;
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
