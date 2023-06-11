#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "handoff/DepartureHandoffResolver.h"

namespace UKControllerPluginTest::Handoff {

    class MockDepartureHandoffResolver : public UKControllerPlugin::Handoff::DepartureHandoffResolver
    {
        public:
        MockDepartureHandoffResolver();
        virtual ~MockDepartureHandoffResolver();
        MOCK_METHOD(
            std::shared_ptr<const UKControllerPlugin::Handoff::ResolvedHandoff>,
            Resolve,
            (const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&),
            (override));
        MOCK_METHOD(
            void, Invalidate, (const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&), (override));
    };
} // namespace UKControllerPluginTest::Handoff
