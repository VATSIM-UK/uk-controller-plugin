#include "countdown/GlobalCountdownSettingFunctions.h"
#include "countdown/TimerConfiguration.h"

using ::testing::Test;
using UKControllerPlugin::Countdown::GetTimerDurationDescription;
using UKControllerPlugin::Countdown::GetTimerDurationKey;
using UKControllerPlugin::Countdown::GetTimerEnabledDescription;
using UKControllerPlugin::Countdown::GetTimerEnabledKey;
using UKControllerPlugin::Countdown::TimerConfiguration;

namespace UKControllerPluginTest {
    namespace Countdown {

        class GlobalCountdownSettingFunctionsTest : public Test
        {
            public:
            TimerConfiguration config = {123, true, 120};
        };

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsEnabledKey)
        {
            EXPECT_EQ(GetTimerEnabledKey(config), "timer123Enabled");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsEnabledKeyUnsignedInteger)
        {
            EXPECT_EQ(GetTimerEnabledKey(123), "timer123Enabled");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsEnabledDescription)
        {
            EXPECT_EQ(GetTimerEnabledDescription(config), "Countdown Timer #123 Enabled");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsEnabledDescriptionUnsignedInteger)
        {
            EXPECT_EQ(GetTimerEnabledDescription(123), "Countdown Timer #123 Enabled");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsDurationKey)
        {
            EXPECT_EQ(GetTimerDurationKey(config), "timer123Duration");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsDurationKeyUnsignedInteger)
        {
            EXPECT_EQ(GetTimerDurationKey(123), "timer123Duration");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsDurationDescription)
        {
            EXPECT_EQ(GetTimerDurationDescription(config), "Countdown Timer #123 Duration");
        }

        TEST_F(GlobalCountdownSettingFunctionsTest, ItReturnsDurationDescriptionUnsignedInteger)
        {
            EXPECT_EQ(GetTimerDurationDescription(123), "Countdown Timer #123 Duration");
        }
    } // namespace Countdown
} // namespace UKControllerPluginTest
