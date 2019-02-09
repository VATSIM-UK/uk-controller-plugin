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
            HoldingAircraft hold = { "BAW123", std::chrono::system_clock::now() };
            EXPECT_TRUE(compare(hold, "BAW124"));
        }

        TEST_F(CompareHoldingAircraftTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldingAircraft hold = { "BAW124", std::chrono::system_clock::now() };
            EXPECT_TRUE(compare("BAW123", hold));
        }

        TEST_F(CompareHoldingAircraftTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldingAircraft hold1 = { "BAW123", std::chrono::system_clock::now() };
            HoldingAircraft hold2 = { "BAW124", std::chrono::system_clock::now() };
            EXPECT_TRUE(compare(hold1, hold2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
