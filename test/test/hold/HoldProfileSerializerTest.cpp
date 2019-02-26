#include "pch/pch.h"
#include "hold/HoldProfileSerializer.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::ProfileJsonValid;
using UKControllerPlugin::Hold::holdProfileSerializerInvalid;
using UKControllerPlugin::Hold::HoldProfile;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldProfileSerializerTest : public Test
        {
            public:
                HoldProfileSerializerTest()
                {
                    testData = {
                        {"id", 1},
                        {"name", "Some Super Cool Hold Profile"},
                        {"user_profile", true},
                        {"holds", {1, 2, 3}}
                    };
                }

                nlohmann::json testData;
        };

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsTrueAllValid)
        {
            EXPECT_TRUE(ProfileJsonValid(this->testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseNoId)
        {
            this->testData.erase("id");
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseIdNotInteger)
        {
            this->testData["id"] = "foo";
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseNoName)
        {
            this->testData.erase("name");
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseNameNotString)
        {
            this->testData["name"] = 123;
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseNoUserProfile)
        {
            this->testData.erase("user_profile");
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseUserProfileNotBoolean)
        {
            this->testData["user_profile"] = 123;
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseNoHolds)
        {
            this->testData.erase("holds");
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseHoldsNotArray)
        {
            this->testData["holds"] = "test";
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ValidJsonReturnsFalseHoldsNotNumeric)
        {
            this->testData["holds"] = { 1, "2", 3 };
            EXPECT_FALSE(ProfileJsonValid(testData));
        }

        TEST_F(HoldProfileSerializerTest, ReturnsInvalidOnInvalidFromJson)
        {
            this->testData["id"] = "nope";
            EXPECT_EQ(holdProfileSerializerInvalid, this->testData.get<HoldProfile>());
        }

        TEST_F(HoldProfileSerializerTest, ReturnsHoldingDataFromJson)
        {
            HoldProfile expected = {
                1,
                "Some Super Cool Hold Profile",
                {1, 2, 3},
                true
            };
            EXPECT_EQ(expected, this->testData.get<HoldProfile>());
        }

        TEST_F(HoldProfileSerializerTest, ReturnsJsonFromHoldingData)
        {
            HoldProfile profile = {
                1,
                "Some Super Cool Hold Profile",
                {1, 2, 3},
                true
            };

            EXPECT_EQ(this->testData, nlohmann::json(profile));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
