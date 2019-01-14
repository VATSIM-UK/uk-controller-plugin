#include "pch/pch.h"
#include "hold/BuildHoldingData.h"
#include "hold/HoldingDataCollection.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::BuildHoldingData;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(BuildHoldingDataTest, ItReturnsEmptyIfDataNotObject)
        {
            nlohmann::json data = "[]"_json;
            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItAddsHoldsWithData)
        {
            nlohmann::json data;
            nlohmann::json hold1;
            hold1["minimum"] = 7000;
            hold1["maximum"] = 15000;
            hold1["inbound"] = 309;
            hold1["direction"] = "right";
            nlohmann::json hold2;
            hold2["minimum"] = 7000;
            hold2["maximum"] = 15000;
            hold2["inbound"] = 285;
            hold2["direction"] = "left";

            data["TIMBA"] = hold1;
            data["WILLO"] = hold2;


            EXPECT_EQ(2, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItAddsTheCorrectHoldingDataRightTurn)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["maximum"] = 15000;
            hold["inbound"] = 309;
            hold["direction"] = "right";

            data["TIMBA"] = hold;
            
            HoldingData expected = { "TIMBA", 7000, 15000, 309, HoldingData::TURN_DIRECTION_RIGHT };

            EXPECT_EQ(expected, BuildHoldingData(data).Get("TIMBA"));
        }

        TEST(BuildHoldingDataTest, ItAddsTheCorrectHoldingDataLeftTurn)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["maximum"] = 15000;
            hold["inbound"] = 309;
            hold["direction"] = "left";

            data["TIMBA"] = hold;

            HoldingData expected = { "TIMBA", 7000, 15000, 309, HoldingData::TURN_DIRECTION_LEFT };

            EXPECT_EQ(expected, BuildHoldingData(data).Get("TIMBA"));
        }

        TEST(BuildHoldingDataTest, ItDoesntAddNonObjects)
        {
            nlohmann::json data;
            data["TIMBA"] = "Test";

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingMinimum)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["maximum"] = 15000;
            hold["inbound"] = 309;
            hold["direction"] = "left";

            data["TIMBA"] = hold;

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingMaximum)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["inbound"] = 309;
            hold["direction"] = "left";

            data["TIMBA"] = hold;

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingInbound)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["maximum"] = 15000;
            hold["direction"] = "left";

            data["TIMBA"] = hold;

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithMissingDirection)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["maximum"] = 15000;
            hold["inbound"] = 309;

            data["TIMBA"] = hold;

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

        TEST(BuildHoldingDataTest, ItDoesntAddWithIncorrectDirection)
        {
            nlohmann::json data;
            nlohmann::json hold;
            hold["minimum"] = 7000;
            hold["maximum"] = 15000;
            hold["inbound"] = 309;
            hold["direction"] = "up";

            data["TIMBA"] = hold;

            EXPECT_EQ(0, BuildHoldingData(data).Count());
        }

    }  // namespace Hold
}  // namespace UKControllerPluginTest
