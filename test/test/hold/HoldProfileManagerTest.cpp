#pragma once
#include "pch/pch.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldProfile;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldProfileManagerTest : public Test
        {
            public:

                HoldProfile profile = { 1, "Test", {1, 2, 3}, true };
                HoldProfileManager manager;
        };

        TEST_F(HoldProfileManagerTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, ItAddsAProfile)
        {
            this->manager.AddProfile(this->profile);
            EXPECT_EQ(1, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, AddingProfileReturnsTrueOnSuccess)
        {
            EXPECT_TRUE(this->manager.AddProfile(this->profile));
        }

        TEST_F(HoldProfileManagerTest, ItDoesntAddProfileDuplicates)
        {
            this->manager.AddProfile(this->profile);
            this->manager.AddProfile(this->profile);
            EXPECT_EQ(1, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, AddingProfileReturnsFalseOnFailure)
        {
            this->manager.AddProfile(this->profile);
            EXPECT_FALSE(this->manager.AddProfile(this->profile));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
