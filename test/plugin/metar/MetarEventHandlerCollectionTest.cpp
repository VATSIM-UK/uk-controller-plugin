#include "pch/pch.h"
#include "metar/MetarEventHandlerCollection.h"
#include "metar/MetarEventHandlerInterface.h"

using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::Metar::MetarEventHandlerInterface;

namespace UKControllerPluginTest {
    namespace EventHandler {

        class TestMetarEventHandler : public UKControllerPlugin::Metar::MetarEventHandlerInterface
        {
            public:
                void NewMetar(std::string station, std::string metar) override
                {
                    this->station = station;
                    this->metar = metar;
                }

                std::string station;
                std::string metar;
        };

        TEST(MetarEventHandlerCollection, NewMetarEventCallsAllHandlers)
        {
            MetarEventHandlerCollection collection;
            std::shared_ptr<TestMetarEventHandler> test1 = std::make_shared<TestMetarEventHandler>();
            std::shared_ptr<TestMetarEventHandler> test2 = std::make_shared<TestMetarEventHandler>();
            collection.RegisterHandler(test1);
            collection.RegisterHandler(test2);

            std::string testStation = "EGKK";
            std::string testMetar = "METARRRRRR";

            collection.NewMetarEvent(testStation, testMetar);

            EXPECT_TRUE(test1->station == testStation);
            EXPECT_TRUE(test1->metar == testMetar);
            EXPECT_TRUE(test2->station == testStation);
            EXPECT_TRUE(test2->metar == testMetar);
        }

        TEST(MetarEventHandlerCollection, StartsEmpty)
        {
            MetarEventHandlerCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST(MetarEventHandlerCollection, CountHandlersReturnsNumberOfRegisteredHandlers)
        {
            MetarEventHandlerCollection collection;
            std::shared_ptr<TestMetarEventHandler> test = std::make_shared<TestMetarEventHandler>();
            collection.RegisterHandler(test);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(test);
            EXPECT_EQ(1, collection.CountHandlers());
        }

        TEST(MetarEventHandlerCollection, RegisterHandlerDoesNotAddDuplicates)
        {
            MetarEventHandlerCollection collection;
            collection.RegisterHandler(std::make_shared<TestMetarEventHandler>());
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(std::make_shared<TestMetarEventHandler>());
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterHandler(std::make_shared<TestMetarEventHandler>());
            EXPECT_EQ(3, collection.CountHandlers());
        }
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
