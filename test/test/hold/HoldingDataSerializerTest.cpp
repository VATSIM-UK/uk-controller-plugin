#include "pch/pch.h"
#include "hold/HoldingDataSerializer.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::JsonValid;
using UKControllerPlugin::Hold::holdSerializerInvalid;
using UKControllerPlugin::Hold::HoldingData;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldingDataSerializerTest : public Test
        {
            public:
                HoldingDataSerializerTest()
                {
                    testData = {
                        {"id", 1},
                        {"fix", "TIMBA"},
                        {"description", "TIMBA LOW"},
                        {"minimum_altitude", 7000},
                        {"maximum_altitude", 15000},
                        {"inbound_heading", 309},
                        {"turn_direction", "right"}
                    };
                }

                nlohmann::json testData;
        };

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsTrueAllValid)
        {
            EXPECT_TRUE(JsonValid(this->testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoId)
        {
            this->testData.erase("id");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseIdNotInteger)
        {
            this->testData["id"] = "foo";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoFix)
        {
            this->testData.erase("fix");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseFixNotString)
        {
            this->testData["fix"] = 123;
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoMinimum)
        {
            this->testData.erase("minimum_altitude");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseMinimumNotInteger)
        {
            this->testData["minimum_altitude"] = "test";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoMaximum)
        {
            this->testData.erase("maximum_altitude");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseMaximumNotInteger)
        {
            this->testData["maximum_altitude"] = "test";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoInbound)
        {
            this->testData.erase("inbound_heading");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseInboundNotInteger)
        {
            this->testData["inbound_heading"] = "test";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoTurn)
        {
            this->testData.erase("turn_direction");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseTurnNotString)
        {
            this->testData["turn_direction"] = 123;
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseTurnNotLeftOrRight)
        {
            this->testData["turn_direction"] = "up";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ReturnsInvalidOnInvalidFromJson)
        {
            this->testData["turn_direction"] = "up";
            EXPECT_EQ(holdSerializerInvalid, this->testData.get<HoldingData>());
        }

        TEST_F(HoldingDataSerializerTest, ReturnsHoldingDataFromJson)
        {
            HoldingData expected = {
                1,
                "TIMBA",
                "TIMBA LOW",
                7000,
                15000,
                309,
                "right"
            };
            EXPECT_EQ(expected, this->testData.get<HoldingData>());
        }

        TEST_F(HoldingDataSerializerTest, ReturnsJsonFromHoldingData)
        {
            HoldingData holdingData = {
                1,
                "TIMBA",
                "TIMBA LOW",
                7000,
                15000,
                309,
                "right"
            };

            EXPECT_EQ(this->testData, nlohmann::json(holdingData));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest