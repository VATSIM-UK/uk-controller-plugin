#include "timer/TimerDisplay.h"
#include "time/SystemClock.h"

using testing::Test;
using UKControllerPlugin::Timer::GetTimerDisplay;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPlugin::Time::SetTestNow;

namespace UKControllerPluginTest {
    namespace Timer {

        class TimerDisplayTest : public Test
        {
            void SetUp() override
            {
                SetTestNow(std::chrono::system_clock::now());
            }
        };

        TEST_F(TimerDisplayTest, ItDisplaysZeroIfTargetTimeIsNow)
        {
            EXPECT_EQ("0:00", GetTimerDisplay(TimeNow()));
        }

        TEST_F(TimerDisplayTest, ItDisplaysZeroIfTargetTimeIsInThePast)
        {
            EXPECT_EQ("0:00", GetTimerDisplay(TimeNow() - std::chrono::seconds(10)));
        }

        TEST_F(TimerDisplayTest, ItDisplaysSecondsLessThanTen)
        {
            EXPECT_EQ("0:07", GetTimerDisplay(TimeNow() + std::chrono::seconds(7)));
        }

        TEST_F(TimerDisplayTest, ItDisplaysSecondsGreaterThanTen)
        {
            EXPECT_EQ("0:39", GetTimerDisplay(TimeNow() + std::chrono::seconds(39)));
        }

        TEST_F(TimerDisplayTest, ItDisplaysMinutesLessThanTen)
        {
            EXPECT_EQ("6:03", GetTimerDisplay(TimeNow() + std::chrono::seconds(3) + std::chrono::minutes(6)));
        }

        TEST_F(TimerDisplayTest, ItDisplaysMinutesGreaterThanTen)
        {
            EXPECT_EQ("14:21", GetTimerDisplay(TimeNow() + std::chrono::seconds(21) + std::chrono::minutes(14)));
        }
    } // namespace Timer
} // namespace UKControllerPluginTest
