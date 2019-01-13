#pragma once
#include "intention/AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPluginTest {
    namespace IntentionCode {
        class MockAirfieldGroup : public UKControllerPlugin::IntentionCode::AirfieldGroup
        {
            public:
                bool HasAirfield(
                    std::string airfield,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route
                ) const {
                    return airfield == "EGLL" || airfield == "EGKK";
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

                    return "NOPE";
                }

            protected:
                bool Initialise(void)
                {
                    return true;
                }
        };
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
