#include "ecfmp/TriggerEcfmpEventLoop.h"

namespace UKControllerPluginTest::ECFMP {
    class TriggerECFMPEventLoopTest : public testing::Test
    {
        public:
        TriggerECFMPEventLoopTest()
            : ecfmp(std::make_shared<::testing::NiceMock<::ECFMP::Mock::Plugin::SdkMock>>()), trigger(ecfmp)
        {
        }

        std::shared_ptr<testing::NiceMock<::ECFMP::Mock::Plugin::SdkMock>> ecfmp;
        UKControllerPlugin::ECFMP::TriggerECFMPEventLoop trigger;
    };

    TEST_F(TriggerECFMPEventLoopTest, ItTriggersTheECFMPEventLoop)
    {
        EXPECT_CALL(*ecfmp, OnEuroscopeTimerTick()).Times(1);
        trigger.TimedEventTrigger();
    }
} // namespace UKControllerPluginTest::ECFMP
