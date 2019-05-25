#include "pch/pch.h"
#include "countdown/TimerConfig.h"

using UKControllerPlugin::Countdown::TimerConfig;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Countdown {

        class TimerConfigTest : public Test
        {

        };


        TEST_F(TimerConfigTest, TestEqualityReturnsTrueIfIdsSame)
        {
            TimerConfig config1 = { 1, false, 120 };
            TimerConfig config2 = { 1, true, 180 };
            EXPECT_TRUE(config1 == config2);
        }

        TEST_F(TimerConfigTest, TestEqualityReturnsFalseIfIdsDifferent)
        {
            TimerConfig config1 = { 1, false, 120 };
            TimerConfig config2 = { 2, false, 120 };
            EXPECT_FALSE(config1 == config2);
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
