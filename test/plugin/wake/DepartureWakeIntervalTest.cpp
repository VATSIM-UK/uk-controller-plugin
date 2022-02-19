#include "wake/DepartureWakeInterval.h"

using UKControllerPlugin::Wake::DepartureWakeInterval;

namespace UKControllerPluginTest::Wake {
    class DepartureWakeIntervalTest : public testing::Test
    {
        public:
        DepartureWakeIntervalTest() : interval(1, 4, "s", false)
        {
        }

        DepartureWakeInterval interval;
    };

    TEST_F(DepartureWakeIntervalTest, ItHasAUnit)
    {
        EXPECT_EQ("s", interval.Unit());
    }

    TEST_F(DepartureWakeIntervalTest, ItHasASubsequentCategory)
    {
        EXPECT_EQ(1, interval.SubsequentCategory());
    }

    TEST_F(DepartureWakeIntervalTest, ItHasAnInterval)
    {
        EXPECT_FLOAT_EQ(4, interval.Value());
    }

    TEST_F(DepartureWakeIntervalTest, ItCanBeIntermediate)
    {
        EXPECT_FALSE(interval.Intermediate());
    }
} // namespace UKControllerPluginTest::Wake
