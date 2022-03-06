#include "wake/ArrivalWakeInterval.h"

using UKControllerPlugin::Wake::ArrivalWakeInterval;

namespace UKControllerPluginTest::Wake {
    class ArrivalWakeIntervalTest : public testing::Test
    {
        public:
        ArrivalWakeIntervalTest() : interval(1, 4.5)
        {
        }

        ArrivalWakeInterval interval;
    };

    TEST_F(ArrivalWakeIntervalTest, ItsUnitIsAlwaysNauticalMiles)
    {
        EXPECT_EQ("nm", interval.Unit());
    }

    TEST_F(ArrivalWakeIntervalTest, ItHasASubsequentCategory)
    {
        EXPECT_EQ(1, interval.SubsequentCategory());
    }

    TEST_F(ArrivalWakeIntervalTest, ItHasAnInterval)
    {
        EXPECT_FLOAT_EQ(4.5, interval.Value());
    }
} // namespace UKControllerPluginTest::Wake
