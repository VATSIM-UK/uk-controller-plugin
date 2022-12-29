#pragma once
#include "intention/AircraftFirExitGenerator.h"

namespace UKControllerPluginTest::IntentionCode {
    class MockAircraftFirExitGenerator : public UKControllerPlugin::IntentionCode::AircraftFirExitGenerator
    {
        public:
        MOCK_METHOD(
            std::shared_ptr<UKControllerPlugin::IntentionCode::AircraftFirExit>,
            Generate,
            (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&),
            (override));
    };
} // namespace UKControllerPluginTest::IntentionCode
