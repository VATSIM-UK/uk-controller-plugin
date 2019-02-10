#include "pch/pch.h"
#include "hold/BuildHoldingData.h"
#include "hold/HoldManager.h"
#include "hold/HoldingData.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::BuildHoldingData;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::ManagedHold;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(BuildHoldingDataTest, ItReturnsEmptyIfDataNotObject)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItAddsHoldsWithData)
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
            EXPECT_EQ(2, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItAddsTheCorrectHoldingDataRightTurn)
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
            
            HoldingData expectedData = {
                1,
                "TIMBA",
                "TIMBA Description",
                7000,
                15000,
                309,
                HoldingData::TURN_DIRECTION_RIGHT
            };
            ManagedHold expectedHold(expectedData);
            EXPECT_EQ(expectedHold, *BuildHoldingData(data)->GetManagedHold(1));
        }

        TEST(BuildHoldingDataTest, ItAddsTheCorrectHoldingDataLeftTurn)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "left";

            data = nlohmann::json::array({ hold });

            HoldingData expectedData = {
                1,
                "TIMBA",
                "TIMBA Description",
                7000,
                15000,
                309,
                HoldingData::TURN_DIRECTION_LEFT
            };
            ManagedHold expectedHold(expectedData);
            EXPECT_EQ(expectedHold, *BuildHoldingData(data)->GetManagedHold(1));
        }

        TEST(BuildHoldingDataTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data = { "Test" };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingMinimum)
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

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingMaximum)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "left";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingInbound)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["turn_direction"] = "left";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingDirection)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithIncorrectDirection)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "up";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingId)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["description"] = "TIMBA";
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "right";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingFix)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["fix"] = "TIMBA Description";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "right";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingDescription)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["id"] = 1;
            hold["description"] = "TIMBA";
            hold["minimum_altitude"] = 7000;
            hold["maximum_altitude"] = 15000;
            hold["inbound_heading"] = 309;
            hold["turn_direction"] = "right";

            data = { hold };

            EXPECT_EQ(0, BuildHoldingData(data)->CountHolds());
        }

        TEST(BuildHoldingDataTest, TheHoldStartsempty)
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
            EXPECT_EQ(0, BuildHoldingData(data)->GetManagedHold(1)->CountHoldingAircraft());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
