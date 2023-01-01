#pragma once
#include "intention/ExitDetermination.h"

namespace UKControllerPluginTest::IntentionCode {
    class MockExitDetermination : public UKControllerPlugin::IntentionCode::ExitDetermination
    {
        public:
        MOCK_METHOD(
            bool,
            AircraftIsExiting,
            (const UKControllerPlugin::Flightplan::FlightplanPoint&,
             const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&),
            (const, override));
    };
} // namespace UKControllerPluginTest::IntentionCode
