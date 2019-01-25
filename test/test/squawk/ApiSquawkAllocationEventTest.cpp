#pragma once
#include "pch/pch.h"
#include "squawk/ApiSquawkAllocationEvent.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocationEvent;

namespace UKControllerPluginTest {
    namespace Squawk {
        TEST(ApiSquawkAllocationEventTest, ItIsEqualIfSquawkAndCallsignMatch)
        {
            ApiSquawkAllocationEvent event1 = { "BAW123", "0123" };
            ApiSquawkAllocationEvent event2 = { "BAW123", "0123" };
            EXPECT_EQ(event1, event2);
        }

        TEST(ApiSquawkAllocationEventTest, ItIsNotEqualIfCallsignDifferent)
        {
            ApiSquawkAllocationEvent event1 = { "BAW123", "0123" };
            ApiSquawkAllocationEvent event2 = { "BAW124", "0123" };
            EXPECT_FALSE(event1 == event2);
        }

        TEST(ApiSquawkAllocationEventTest, ItIsNotEqualIfSquawkDifferentDifferent)
        {
            ApiSquawkAllocationEvent event1 = { "BAW123", "0123" };
            ApiSquawkAllocationEvent event2 = { "BAW123", "4567" };
            EXPECT_FALSE(event1 == event2);
        }

        TEST(ApiSquawkAllocationEventTest, LessThanReturnsTrueIfCallsignLessThan)
        {
            ApiSquawkAllocationEvent event1 = { "BAW123", "0123" };
            ApiSquawkAllocationEvent event2 = { "BAW124", "0123" };
            EXPECT_FALSE(event1 == event2);
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
