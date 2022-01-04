#include "metar/MetarEventHandlerCollection.h"
#include "metar/MetarEventHandlerInterface.h"
#include "metar/ParsedMetar.h"

using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::Metar::MetarEventHandlerInterface;
using UKControllerPlugin::Metar::ParsedMetar;

namespace UKControllerPluginTest::Metar {

    class TestMetarEventHandler : public MetarEventHandlerInterface
    {
        public:
        void MetarUpdated(const ParsedMetar& metar) override
        {
            this->called = true;
        }

        bool called = false;
    };

    class MetarEventHandlerCollectionTest : public testing::Test
    {
        public:
        MetarEventHandlerCollectionTest()
            : handler1(std::make_shared<TestMetarEventHandler>()), handler2(std::make_shared<TestMetarEventHandler>())
        {
        }
        std::shared_ptr<TestMetarEventHandler> handler1;
        std::shared_ptr<TestMetarEventHandler> handler2;
        MetarEventHandlerCollection collection;
    };

    TEST_F(MetarEventHandlerCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.CountHandlers());
    }

    TEST_F(MetarEventHandlerCollectionTest, ItAddsHandlers)
    {
        collection.RegisterHandler(handler1);
        collection.RegisterHandler(handler2);
        EXPECT_EQ(2, collection.CountHandlers());
    }

    TEST_F(MetarEventHandlerCollectionTest, ItDoesntDuplicateHandlers)
    {
        collection.RegisterHandler(handler1);
        collection.RegisterHandler(handler1);
        collection.RegisterHandler(handler1);
        collection.RegisterHandler(handler2);
        collection.RegisterHandler(handler2);
        collection.RegisterHandler(handler2);
        EXPECT_EQ(2, collection.CountHandlers());
    }

    TEST_F(MetarEventHandlerCollectionTest, NewMetarEventCallsHandlers)
    {
        collection.RegisterHandler(handler1);
        collection.RegisterHandler(handler2);

        std::shared_ptr<ParsedMetar> parsed;
        collection.UpdatedMetarEvent(*parsed);

        EXPECT_TRUE(handler1->called);
        EXPECT_TRUE(handler2->called);
    }
} // namespace UKControllerPluginTest::Metar
