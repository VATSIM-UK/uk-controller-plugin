#pragma once
#include "pch/pch.h"
#include "hold/HoldingAircraft.h"
#include "hold/CompareHoldingAircraft.h"

using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::CompareHoldingAircraft;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldingAircraftTest : public Test
        {
            public:
                CompareHoldingAircraft compare;
        };

        TEST_F(CompareHoldingAircraftTest, LessThanStringReturnsTrueIfLessThan)
        {
            HoldingAircraft hold = { "BAW123", "TIMBA", 7000, 8000 };
            EXPECT_TRUE(compare(hold, "BAW124"));
        }

        TEST_F(CompareHoldingAircraftTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldingAircraft hold = { "BAW124", "TIMBA", 7000, 8000 };
            EXPECT_TRUE(compare("BAW123", hold));
        }

        TEST_F(CompareHoldingAircraftTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldingAircraft hold1 = { "BAW123", "TIMBA", 7000, 8000 };
            HoldingAircraft hold2 = { "BAW124", "TIMBA", 7000, 8000 };
            EXPECT_TRUE(compare(hold1, hold2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
