#include "flightrule/FlightRule.h"
#include "flightrule/FlightRuleCollection.h"
#include "flightrule/FlightRuleCollectionFactory.h"

using UKControllerPlugin::FlightRules::CreateCollection;
using UKControllerPlugin::FlightRules::FlightRuleValid;

namespace UKControllerPluginTest::FlightRules {
    class FlightRuleCollectionFactoryTest : public testing::Test
    {
    };

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsValid)
    {
        nlohmann::json data{
            {"id", 1},
            {"euroscope_key", "I"},
            {"description", "IFR"},
        };
        EXPECT_TRUE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfNoId)
    {
        nlohmann::json data{
            {"euroscope_key", "I"},
            {"description", "IFR"},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfIdNotInteger)
    {
        nlohmann::json data{
            {"id", "abc"},
            {"euroscope_key", "I"},
            {"description", "IFR"},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfKeyMissing)
    {
        nlohmann::json data{
            {"id", 1},
            {"description", "IFR"},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfKeyNotString)
    {
        nlohmann::json data{
            {"id", 1},
            {"euroscope_key", 123},
            {"description", "IFR"},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfNoDescription)
    {
        nlohmann::json data{
            {"id", 1},
            {"euroscope_key", "I"},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfDescriptionNotString)
    {
        nlohmann::json data{
            {"id", 1},
            {"euroscope_key", "I"},
            {"description", 123},
        };
        EXPECT_FALSE(FlightRuleValid(data));
    }

    TEST_F(FlightRuleCollectionFactoryTest, AFlightRuleIsInvalidIfNotObject)
    {
        nlohmann::json data{
            {"id", 1},
            {"euroscope_key", "I"},
            {"description", "IFR"},
        };
        EXPECT_FALSE(FlightRuleValid(nlohmann::json::array()));
    }

    TEST_F(FlightRuleCollectionFactoryTest, ItReturnsACollection)
    {
        nlohmann::json data = nlohmann::json::array();
        data.push_back({
            {"id", 1},
            {"euroscope_key", "I"},
            {"description", "IFR"},
        });
        data.push_back({
            {"id", 2},
            {"euroscope_key", "V"},
            {"description", "VFR"},
        });

        auto collection = CreateCollection(data);
        EXPECT_EQ(2, collection->Count());
        auto rule1 = collection->Get(1);
        EXPECT_EQ(1, rule1->id);
        EXPECT_EQ("I", rule1->euroscopeKey);
        EXPECT_EQ("IFR", rule1->description);

        auto rule2 = collection->Get(2);
        EXPECT_EQ(2, rule2->id);
        EXPECT_EQ("V", rule2->euroscopeKey);
        EXPECT_EQ("VFR", rule2->description);
    }

    TEST_F(FlightRuleCollectionFactoryTest, ItIgnoresBadRules)
    {
        nlohmann::json data = nlohmann::json::array();
        data.push_back({
            {"id", 1},
            {"euroscope_key", "I"},
            {"description", "IFR"},
        });
        data.push_back({
            {"id", 2},
            {"euroscope_key", 123},
            {"description", "VFR"},
        });

        auto collection = CreateCollection(data);
        EXPECT_EQ(1, collection->Count());
        auto rule1 = collection->Get(1);
        EXPECT_EQ(1, rule1->id);
        EXPECT_EQ("I", rule1->euroscopeKey);
        EXPECT_EQ("IFR", rule1->description);
    }

    TEST_F(FlightRuleCollectionFactoryTest, ItReturnsEmptyCollectionIfDependencyNotArray)
    {
        auto collection = CreateCollection(nlohmann::json::object());
        EXPECT_EQ(0, collection->Count());
    }
} // namespace UKControllerPluginTest::FlightRules
