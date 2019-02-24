#include "pch/pch.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::HoldProfile;

namespace UKControllerPluginTest {
    namespace Hold {
        
        TEST(HoldProfileTest, EqualityReturnsTrueIfIdentical)
        {
            HoldProfile profile1 = { 1, "test", {1, 2, 3} };
            HoldProfile profile2 = { 1, "test", {1, 2, 3} };
            EXPECT_TRUE(profile1 == profile2);
        }

        TEST(HoldProfileTest, EqualityReturnsFalseDifferentId)
        {
            HoldProfile profile1 = { 1, "test", {1, 2, 3} };
            HoldProfile profile2 = { 2, "test", {1, 2, 3} };
            EXPECT_FALSE(profile1 == profile2);
        }

        TEST(HoldProfileTest, EqualityReturnsFalseDifferentName)
        {
            HoldProfile profile1 = { 1, "test", {1, 2, 3} };
            HoldProfile profile2 = { 1, "test2", {1, 2, 3} };
            EXPECT_FALSE(profile1 == profile2);
        }

        TEST(HoldProfileTest, EqualityReturnsFalseDifferentHolds)
        {
            HoldProfile profile1 = { 1, "test", {1, 2, 3} };
            HoldProfile profile2 = { 1, "test", {1, 2, 3, 4} };
            EXPECT_FALSE(profile1 == profile2);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest