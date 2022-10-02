#include "countdown/TimerConfiguration.h"

using testing::Test;
using UKControllerPlugin::Countdown::TimerConfiguration;

namespace UKControllerPluginTest {
    namespace Countdown {

        class TimerConfigurationTest : public Test
        {
        };

        TEST_F(TimerConfigurationTest, TestEqualityReturnsTrueIfIdsSame)
        {
            TimerConfiguration config1 = {1, false, 120};
            TimerConfiguration config2 = {1, true, 180};
            EXPECT_TRUE(config1 == config2);
        }

        TEST_F(TimerConfigurationTest, TestEqualityReturnsFalseIfIdsDifferent)
        {
            TimerConfiguration config1 = {1, false, 120};
            TimerConfiguration config2 = {2, false, 120};
            EXPECT_FALSE(config1 == config2);
        }
    } // namespace Countdown
} // namespace UKControllerPluginTest
