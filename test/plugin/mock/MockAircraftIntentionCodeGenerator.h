#pragma once
#include "intention/AircraftIntentionCodeGenerator.h"

namespace UKControllerPluginTest::IntentionCode {

    class MockAircraftIntentionCodeGenerator : public UKControllerPlugin::IntentionCode::AircraftIntentionCodeGenerator
    {
        public:
        MockAircraftIntentionCodeGenerator();
        virtual ~MockAircraftIntentionCodeGenerator();
        MOCK_METHOD(
            std::shared_ptr<UKControllerPlugin::IntentionCode::AircraftIntentionCode>,
            Generate,
            (const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
             const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&),
            (override));
    };
} // namespace UKControllerPluginTest::IntentionCode
