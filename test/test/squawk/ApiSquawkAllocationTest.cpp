#pragma once
#include "pch/pch.h"
#include "squawk/ApiSquawkAllocation.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocation;

namespace UKControllerPluginTest {
    namespace Squawk {
        TEST(ApiSquawkAllocationTest, ItIsEqualIfSquawkAndCallsignMatch)
        {
            ApiSquawkAllocation event1 = { "BAW123", "0123" };
            ApiSquawkAllocation event2 = { "BAW123", "0123" };
            EXPECT_EQ(event1, event2);
        }

        TEST(ApiSquawkAllocationTest, ItIsNotEqualIfCallsignDifferent)
        {
            ApiSquawkAllocation event1 = { "BAW123", "0123" };
            ApiSquawkAllocation event2 = { "BAW124", "0123" };
            EXPECT_FALSE(event1 == event2);
        }

        TEST(ApiSquawkAllocationTest, ItIsNotEqualIfSquawkDifferentDifferent)
        {
            ApiSquawkAllocation event1 = { "BAW123", "0123" };
            ApiSquawkAllocation event2 = { "BAW123", "4567" };
            EXPECT_FALSE(event1 == event2);
        }

        TEST(ApiSquawkAllocationTest, LessThanReturnsTrueIfCallsignLessThan)
        {
            ApiSquawkAllocation event1 = { "BAW123", "0123" };
            ApiSquawkAllocation event2 = { "BAW124", "0123" };
            EXPECT_FALSE(event1 == event2);
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
