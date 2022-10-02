#include "time/SystemClock.h"

using ::testing::Test;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest {
    namespace Time {

        class SystemClockTest : public Test
        {
        };

        TEST_F(SystemClockTest, ItReturnsChronoNowIfNoTestTimeSet)
        {
            int64_t seconds =
                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - TimeNow()).count();

            EXPECT_TRUE(seconds < 3);
        }

        TEST_F(SystemClockTest, ItReturnsTestNowIfSet)
        {
            std::chrono::system_clock::time_point testNow = std::chrono::system_clock::now() + std::chrono::minutes(5);
            SetTestNow(testNow);
            EXPECT_EQ(testNow, TimeNow());
        }
    } // namespace Time
} // namespace UKControllerPluginTest
