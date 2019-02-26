#include "pch/pch.h"
#include "hold/HoldProfileManagerFactory.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::CreateHoldProfileManager;
using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Hold::HoldProfileManager;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(HoldProfileManagerFactory, ItReturnsEmptyIfDataEmpty)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, CreateHoldProfileManager(data)->CountProfiles());
        }

        TEST(HoldProfileManagerFactory, ItReturnsEmptyIfDataNotArray)
        {
            nlohmann::json data = nlohmann::json::object();
            EXPECT_EQ(0, CreateHoldProfileManager(data)->CountProfiles());
        }

        TEST(HoldProfileManagerFactory, ItAddsProfilesWithData)
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
            EXPECT_EQ(2, CreateHoldProfileManager(data)->CountProfiles());
        }

        TEST(HoldProfileManagerFactory, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, CreateHoldProfileManager(data)->CountProfiles());
        }

        TEST(HoldProfileManagerFactory, ItDoesntAddInvalidData)
        {
            nlohmann::json data;
            nlohmann::json profile;
            profile["id"] = 1;
            profile["name"] = 123;
            profile["holds"] = { 1, 2, 3 };

            data = { profile };

            EXPECT_EQ(0, CreateHoldProfileManager(data)->CountProfiles());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
