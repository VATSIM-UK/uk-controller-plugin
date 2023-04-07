#pragma once
#include "wake/WakeCategoryMapperInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeCategoryMapperInterface;

namespace UKControllerPluginTest::Wake {
    class MockWakeCategoryMapper : public WakeCategoryMapperInterface
    {
        public:
        MockWakeCategoryMapper();
        virtual ~MockWakeCategoryMapper();
        MOCK_METHOD(
            std::shared_ptr<WakeCategory>, MapForFlightplan, (const EuroScopeCFlightPlanInterface&), (override));
    };
} // namespace UKControllerPluginTest::Wake
