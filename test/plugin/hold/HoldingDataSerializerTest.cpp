#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldingDataSerializer.h"
#include "hold/HoldingData.h"
#include "bootstrap/PersistenceContainer.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Hold::from_json_with_restrictions;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::holdSerializerInvalid;
using UKControllerPlugin::Hold::JsonValid;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldingDataSerializerTest : public Test
        {
            public:
            HoldingDataSerializerTest()
            {
                nlohmann::json restriction = {{"type", "level-block"}, {"levels", nlohmann::json::array({7000, 9000})}};

                nlohmann::json deemedSeparatedHold = {{"hold_id", 2}, {"vsl_insert_distance", 2}};

                testData = {
                    {"id", 1},
                    {"fix", "TIMBA"},
                    {"description", "TIMBA LOW"},
                    {"minimum_altitude", 7000},
                    {"maximum_altitude", 15000},
                    {"inbound_heading", 309},
                    {"turn_direction", "right"},
                    {"restrictions", nlohmann::json::array({restriction})},
                    {"deemed_separated_holds", nlohmann::json::array({deemedSeparatedHold})},
                };
            }

            PersistenceContainer container;

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

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoRestrictions)
        {
            this->testData.erase("restrictions");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseRestrictionsNotArray)
        {
            this->testData["restrictions"] = "lol";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseNoDeemedSeparated)
        {
            this->testData.erase("deemed_separated_holds");
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ValidJsonReturnsFalseDeemedSeparatedNotArray)
        {
            this->testData["deemed_separated_holds"] = "lol";
            EXPECT_FALSE(JsonValid(testData));
        }

        TEST_F(HoldingDataSerializerTest, ReturnsInvalidOnInvalidFromJson)
        {
            this->testData["turn_direction"] = "up";
            EXPECT_EQ(holdSerializerInvalid, this->testData.get<HoldingData>());
        }

        TEST_F(HoldingDataSerializerTest, ReturnsHoldingDataFromJson)
        {
            HoldingData actual = this->testData.get<HoldingData>();
            EXPECT_EQ(1, actual.identifier);
            EXPECT_EQ("TIMBA", actual.fix);
            EXPECT_EQ("TIMBA LOW", actual.description);
            EXPECT_EQ(7000, actual.minimum);
            EXPECT_EQ(15000, actual.maximum);
            EXPECT_EQ(309, actual.inbound);
            EXPECT_EQ("right", actual.turnDirection);
        }

        TEST_F(HoldingDataSerializerTest, ReturnsHoldingDataWithRestrictionsFromJson)
        {
            HoldingData actual;
            from_json_with_restrictions(this->testData, actual, this->container);
            EXPECT_EQ(1, actual.identifier);
            EXPECT_EQ("TIMBA", actual.fix);
            EXPECT_EQ("TIMBA LOW", actual.description);
            EXPECT_EQ(7000, actual.minimum);
            EXPECT_EQ(15000, actual.maximum);
            EXPECT_EQ(309, actual.inbound);
            EXPECT_EQ("right", actual.turnDirection);
            EXPECT_EQ(1, actual.restrictions.size());
            EXPECT_TRUE((*actual.restrictions.cbegin())->LevelRestricted(7000));
            EXPECT_TRUE((*actual.restrictions.cbegin())->LevelRestricted(9000));
            EXPECT_EQ(2, (*actual.deemedSeparatedHolds.cbegin())->identifier);
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
