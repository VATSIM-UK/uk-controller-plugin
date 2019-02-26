#include "pch/pch.h"
#include "hold/HoldManagerFactory.h"
#include "hold/HoldManager.h"
#include "hold/HoldingData.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::CreateHoldManager;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::ManagedHold;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(HoldManagerFactoryTest, ItReturnsEmptyIfDataEmpty)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, CreateHoldManager(data)->CountHolds());
        }

        TEST(HoldManagerFactoryTest, ItReturnsEmptyIfDataNotArray)
        {
            nlohmann::json data = nlohmann::json::object();
            EXPECT_EQ(0, CreateHoldManager(data)->CountHolds());
        }

        TEST(HoldManagerFactoryTest, ItAddsHoldsWithData)
        {
            nlohmann::json data;
            nlohmann::json hold1;
            hold1["id"] = 1;
            hold1["description"] = "TIMBA";
            hold1["fix"] = "TIMBA";
            hold1["minimum_altitude"] = 7000;
            hold1["maximum_altitude"] = 15000;
            hold1["inbound_heading"] = 309;
            hold1["turn_direction"] = "right";
            nlohmann::json hold2;
            hold2["id"] = 2;
            hold2["description"] = "WILLO";
            hold2["fix"] = "WILLO";
            hold2["minimum_altitude"] = 7000;
            hold2["maximum_altitude"] = 15000;
            hold2["inbound_heading"] = 285;
            hold2["turn_direction"] = "left";

            data = { hold1, hold2 };
            EXPECT_EQ(2, CreateHoldManager(data)->CountHolds());
        }

        TEST(HoldManagerFactoryTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, CreateHoldManager(data)->CountHolds());
        }

        TEST(HoldManagerFactoryTest, ItDoesntAddInvalidData)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "left";

            data = { hold };

            EXPECT_EQ(0, CreateHoldManager(data)->CountHolds());
        }

        TEST(HoldManagerFactoryTest, TheHoldStartsEmpty)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "right";

            data = nlohmann::json::array({ hold });
            EXPECT_EQ(0, CreateHoldManager(data)->GetManagedHold(1)->CountHoldingAircraft());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
