#include "wake/ArrivalWakeInterval.h"
#include "wake/DepartureWakeInterval.h"
#include "wake/WakeIntervalFormatter.h"

using UKControllerPlugin::Wake::ArrivalWakeInterval;
using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::FormatInterval;

namespace UKControllerPluginTest::Wake {
    class WakeIntervalFormatterTest : public testing::Test
    {
    };

    TEST_F(WakeIntervalFormatterTest, ItReturnsDefaultUnknownUnit)
    {
        DepartureWakeInterval interval{1, 60, "mm", true};
        EXPECT_EQ(L"--", FormatInterval(interval));
    }

    TEST_F(WakeIntervalFormatterTest, ItReturnsUnitNauticalMiles)
    {
        DepartureWakeInterval interval{1, 60, "nm", true};
        EXPECT_EQ(L"60.0 nm", FormatInterval(interval));
    }

    TEST_F(WakeIntervalFormatterTest, ItReturnsUnitDecimalNauticalMiles)
    {
        ArrivalWakeInterval interval{1, 60.5};
        EXPECT_EQ(L"60.5 nm", FormatInterval(interval));
    }

    TEST_F(WakeIntervalFormatterTest, ItReturnsMinutes)
    {
        DepartureWakeInterval interval{1, 120, "s", true};
        EXPECT_EQ(L"2 mins", FormatInterval(interval));
    }

    TEST_F(WakeIntervalFormatterTest, ItReturnsSeconds)
    {
        DepartureWakeInterval interval{1, 150, "s", true};
        EXPECT_EQ(L"150 secs", FormatInterval(interval));
    }
} // namespace UKControllerPluginTest::Wake
