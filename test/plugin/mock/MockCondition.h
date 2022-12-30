#pragma once
#include "intention/Condition.h"

namespace UKControllerPluginTest::IntentionCode {
    class MockCondition : public UKControllerPlugin::IntentionCode::Condition
    {
        public:
        MOCK_METHOD(
            bool,
            Passes,
            (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
             UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&),
            (override));
    };
} // namespace UKControllerPluginTest::IntentionCode
