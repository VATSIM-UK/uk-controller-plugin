#include "pch/pch.h"
#include "hold/HoldProfileManagerFactory.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldProfile.h"
#include "mock/MockApiInterface.h"

using UKControllerPlugin::Hold::CreateHoldProfileManager;
using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPluginTest::Api::MockApiInterface;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldProfileManagerFactoryTest : public Test
        {
            public:
                NiceMock<MockApiInterface> mockApi;
        };

        TEST_F(HoldProfileManagerFactoryTest, ItReturnsEmptyIfDataEmpty)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, CreateHoldProfileManager(data, this->mockApi)->CountProfiles());
        }

        TEST_F(HoldProfileManagerFactoryTest, ItReturnsEmptyIfDataNotArray)
        {
            nlohmann::json data = nlohmann::json::object();
            EXPECT_EQ(0, CreateHoldProfileManager(data, this->mockApi)->CountProfiles());
        }

        TEST_F(HoldProfileManagerFactoryTest, ItAddsProfilesWithData)
        {
            nlohmann::json data;
            nlohmann::json profile1;
            profile1["id"] = 1;
            profile1["name"] = "Gatwick Director";
            profile1["holds"] = {1, 2, 3};
            nlohmann::json profile2;
            profile2["id"] = 2;
            profile2["name"] = "Heathrow Director";
            profile2["holds"] = { 1, 2, 3 };

            data = { profile1, profile2 };
            EXPECT_EQ(2, CreateHoldProfileManager(data, this->mockApi)->CountProfiles());
        }

        TEST_F(HoldProfileManagerFactoryTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, CreateHoldProfileManager(data, this->mockApi)->CountProfiles());
        }

        TEST_F(HoldProfileManagerFactoryTest, ItDoesntAddInvalidData)
        {
            nlohmann::json data;
            nlohmann::json profile;
            profile["id"] = 1;
            profile["name"] = 123;
            profile["holds"] = { 1, 2, 3 };

            data = { profile };

            EXPECT_EQ(0, CreateHoldProfileManager(data, this->mockApi)->CountProfiles());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
