#pragma once
#include "pch/pch.h"
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"

using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::CompareHolds;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldsTest : public Test
        {
            public:
                CompareHolds compare;
        };

        TEST_F(CompareHoldsTest, LessThanUnsignedIntReturnsTrueIfLessThan)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {} };
            EXPECT_TRUE(compare(hold, 2));
        }

        TEST_F(CompareHoldsTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {} };
            EXPECT_TRUE(compare(0, hold));
        }

        TEST_F(CompareHoldsTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldingData hold1 = { 1, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {} };
            HoldingData hold2 = { 2, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {} };
            EXPECT_TRUE(compare(hold1, hold2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
