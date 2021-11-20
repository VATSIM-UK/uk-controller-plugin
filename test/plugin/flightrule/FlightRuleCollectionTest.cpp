#include "flightrule/FlightRule.h"
#include "flightrule/FlightRuleCollection.h"

using UKControllerPlugin::FlightRules::FlightRule;
using UKControllerPlugin::FlightRules::FlightRuleCollection;

namespace UKControllerPluginTest::FlightRules {
    class FlightRuleCollectionTest : public testing::Test
    {
        public:
        FlightRuleCollectionTest()
            : rule1(std::make_shared<FlightRule>(1, "I", "IFR")), rule2(std::make_shared<FlightRule>(2, "V", "VFR"))
        {
        }

        std::shared_ptr<FlightRule> rule1;
        std::shared_ptr<FlightRule> rule2;
        FlightRuleCollection collection;
    };

    TEST_F(FlightRuleCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(FlightRuleCollectionTest, ItAddsRules)
    {
        collection.Add(rule1);
        collection.Add(rule2);
        EXPECT_EQ(2, collection.Count());
        EXPECT_EQ(rule1, collection.Get(1));
        EXPECT_EQ(rule2, collection.Get(2));
    }

    TEST_F(FlightRuleCollectionTest, ItDoesntAddDuplicateRules)
    {
        collection.Add(rule1);
        collection.Add(rule1);
        collection.Add(rule1);
        collection.Add(rule2);
        collection.Add(rule2);
        collection.Add(rule2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(FlightRuleCollectionTest, ItReturnsNullptrOnRuleNotFound)
    {
        collection.Add(rule1);
        collection.Add(rule2);
        EXPECT_EQ(2, collection.Count());
        EXPECT_EQ(nullptr, collection.Get(3));
    }
} // namespace UKControllerPluginTest::FlightRules
