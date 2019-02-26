#pragma once
#include "pch/pch.h"
#include "hold/HoldProfile.h"
#include "hold/CompareHoldProfileName.h"

using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Hold::CompareHoldProfileName;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldProfileNameTest : public Test
        {
            public:
            CompareHoldProfileName compare;
        };

        TEST_F(CompareHoldProfileNameTest, LessThanStringReturnsTrueIfLessThan)
        {
            HoldProfile profile = { 1, "Test Name", {1,2}, true };
            EXPECT_TRUE(compare(profile, "Very Good Test Name"));
        }

        TEST_F(CompareHoldProfileNameTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldProfile profile = { 1, "Very Good Test Name", {1,2}, true };
            EXPECT_TRUE(compare("Test Name", profile));
        }

        TEST_F(CompareHoldProfileNameTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldProfile profile1 = { 1, "Test Name", {1,2}, true };
            HoldProfile profile2 = { 1, "Very Good Test Name", {1,2}, true };
            EXPECT_TRUE(compare(profile1, profile2));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
