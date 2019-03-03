#pragma once
#include "pch/pch.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldProfile.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiException;
using testing::NiceMock;
using testing::Test;
using ::testing::Throw;
using ::testing::Return;
using testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldProfileManagerTest : public Test
        {
            public:
                HoldProfileManagerTest()
                    : manager(mockApi)
                {

                }

                NiceMock<MockApiInterface> mockApi;
                HoldProfile profile = { 1, "Test", {1, 2, 3} };
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

        TEST_F(HoldProfileManagerTest, GetProfileByNameReturnsInvalidNotFound)
        {
            this->manager.AddProfile(this->profile);
            EXPECT_TRUE(this->manager.invalidProfile == this->manager.GetProfileByName("Foo"));
        }

        TEST_F(HoldProfileManagerTest, GetProfileByNameReturnsProfileIfFound)
        {
            this->manager.AddProfile(this->profile);
            EXPECT_TRUE(this->profile == this->manager.GetProfileByName("Test"));
        }

        TEST_F(HoldProfileManagerTest, SavingNewProfileDoesntCreateDuplicates)
        {
            this->manager.AddProfile(this->profile);
            EXPECT_FALSE(this->manager.SaveNewProfile("Test", { 1, 2 }));
            EXPECT_EQ(1, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, SavingNewProfileReturnsFalseOnApiException)
        {
            ON_CALL(this->mockApi, CreateUserHoldProfile(_, _))
                .WillByDefault(Throw(ApiException("Test")));

            EXPECT_FALSE(this->manager.SaveNewProfile("Test", { 1, 2 }));
        }

        TEST_F(HoldProfileManagerTest, SavingNewProfileReturnsTrueOnProfileCreation)
        {
            ON_CALL(this->mockApi, CreateUserHoldProfile(_, _))
                .WillByDefault(Return(2));

            EXPECT_TRUE(this->manager.SaveNewProfile("Test", { 1, 2 }));
        }

        TEST_F(HoldProfileManagerTest, SavingProfileToTheApiAddsItToManager)
        {
            std::set<unsigned int> expectedHolds = { 1,2 };
            ON_CALL(this->mockApi, CreateUserHoldProfile("Test", expectedHolds))
                .WillByDefault(Return(2));

            this->manager.SaveNewProfile("Test", { 1, 2 });

            EXPECT_EQ(1, this->manager.CountProfiles());
            HoldProfile expectedProfile = { 2, "Test", {1, 2} };
            EXPECT_TRUE(expectedProfile == this->manager.GetProfileByName("Test"));
        }

        TEST_F(HoldProfileManagerTest, UpdatingProfileDoesntUpdateNonExisting)
        {
            EXPECT_FALSE(this->manager.UpdateProfile("Test", "Test 2", { 1, 2 }));
        }

        TEST_F(HoldProfileManagerTest, UpdatingProfileReturnsFalseOnApiException)
        {
            EXPECT_CALL(this->mockApi, UpdateUserHoldProfile(_, _, _))
                .Times(1)
                .WillOnce(Throw(ApiException("Test")));

            this->manager.AddProfile(this->profile);
            EXPECT_FALSE(this->manager.UpdateProfile("Test", "Test 2", { 1, 2 }));
        }

        TEST_F(HoldProfileManagerTest, UpdatingProfileReturnsTrueOnSuccess)
        {
            EXPECT_CALL(this->mockApi, UpdateUserHoldProfile(_, _, _))
                .Times(1);

            this->manager.AddProfile(this->profile);
            EXPECT_TRUE(this->manager.UpdateProfile("Test", "Test 2", { 1, 2 }));
        }

        TEST_F(HoldProfileManagerTest, UpdatingProfileUpdatesInApiAndManager)
        {
            this->manager.AddProfile(this->profile);
            std::set<unsigned int> expectedHolds = { 1, 2, 3};
            ON_CALL(this->mockApi, CreateUserHoldProfile("Test 2", expectedHolds))
                .WillByDefault(Return(2));


            this->manager.UpdateProfile("Test", "Test 2", { 1, 2, 3 });
            EXPECT_EQ(1, this->manager.CountProfiles());
            HoldProfile expectedProfile = { 1, "Test 2", {1, 2, 3} };
            EXPECT_TRUE(expectedProfile == this->manager.GetProfileByName("Test 2"));
        }

        TEST_F(HoldProfileManagerTest, DeletingProfileDoesntDeleteNonExisting)
        {
            EXPECT_FALSE(this->manager.DeleteProfile("Test"));
        }

        TEST_F(HoldProfileManagerTest, DeletingProfileDoesntDeleteOnApiException)
        {
            ON_CALL(this->mockApi, DeleteUserHoldProfile(_))
                .WillByDefault(Throw(ApiException("Test")));

            this->manager.AddProfile(this->profile);
            EXPECT_FALSE(this->manager.DeleteProfile("Test"));
            EXPECT_EQ(1, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, DeletingProfileRemovesFromApiAndLocal)
        {
            EXPECT_CALL(this->mockApi, DeleteUserHoldProfile(1))
                .Times(1);

            this->manager.AddProfile(this->profile);
            this->manager.DeleteProfile("Test");
            EXPECT_EQ(0, this->manager.CountProfiles());
        }

        TEST_F(HoldProfileManagerTest, DeletingProfileReturnsTruOnSuccess)
        {
            EXPECT_CALL(this->mockApi, DeleteUserHoldProfile(1))
                .Times(1);

            this->manager.AddProfile(this->profile);
            EXPECT_TRUE(this->manager.DeleteProfile("Test"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
