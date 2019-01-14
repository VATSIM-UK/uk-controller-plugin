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

        TEST_F(CompareHoldsTest, LessThanStringReturnsTrueIfLessThan)
        {
            HoldingData hold = { "TIMBA", 6000, 15000, 50, 0 };
            EXPECT_TRUE(compare(hold, "TIMBB"));
        }

        TEST_F(CompareHoldsTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldingData hold = { "TIMBB", 6000, 15000, 50, 0 };
            EXPECT_TRUE(compare("TIMBA", hold));
        }

        TEST_F(CompareHoldsTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldingData hold1 = { "TIMBA", 6000, 15000, 50, 0 };
            HoldingData hold2 = { "TIMBB", 6000, 15000, 50, 0 };
            EXPECT_TRUE(compare(hold1, hold2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
