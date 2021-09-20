#pragma once
#include "intention/AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPluginTest {
    namespace IntentionCode {
        class MockAirfieldGroup : public UKControllerPlugin::IntentionCode::AirfieldGroup
        {
            public:
            explicit MockAirfieldGroup(bool applicableToController = true)
            {
                this->applicableToController = applicableToController;
            }

            bool AppliesToController(
                const std::string& callsign,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const override
            {
                return this->applicableToController;
            }

            bool HasAirfield(
                const std::string& airfield,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const override
            {
                return airfield == "EGLL" || airfield == "EGKK" || airfield == "EGNX";
            }

            std::string GetIntentionCodeForGroup(
                const std::string& airfield,
                UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const override
            {
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
            bool Initialise() override
            {
                return true;
            }

            bool applicableToController;
        };
    } // namespace IntentionCode
} // namespace UKControllerPluginTest
