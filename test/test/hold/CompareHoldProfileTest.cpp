#pragma once
#include "pch/pch.h"
#include "hold/HoldProfile.h"
#include "hold/CompareHoldProfile.h"

using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Hold::CompareHoldProfile;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldProfileNameTest : public Test
        {
            public:
            CompareHoldProfile compare;
        };

        TEST_F(CompareHoldProfileNameTest, LessThanIntReturnsTrueIfLessThan)
        {
            HoldProfile profile = { 1, "Test Name", { 1, 2 } };
            EXPECT_TRUE(compare(profile, 2));
        }

        TEST_F(CompareHoldProfileNameTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldProfile profile = { 1, "Very Good Test Name", { 1, 2 } };
            EXPECT_TRUE(compare(0, profile));
        }

        TEST_F(CompareHoldProfileNameTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldProfile profile1 = { 1, "Test Name", { 1, 2 } };
            HoldProfile profile2 = { 2, "Test Name", { 1, 2 } };
            EXPECT_TRUE(compare(profile1, profile2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
