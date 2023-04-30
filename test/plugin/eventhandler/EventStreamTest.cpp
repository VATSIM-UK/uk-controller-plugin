#include "eventhandler/EventStream.h"
#include "eventhandler/EventHandler.h"
#include "test/EventBusTestCase.h"

using UKControllerPlugin::EventHandler::EventStream;

namespace UKControllerPluginTest::EventHandler {
    class EventStreamTest : public testing::Test
    {
        public:
        EventStream<int> stream;
    };

    class MockHandler : public UKControllerPlugin::EventHandler::EventHandler<int>
    {
        public:
        void OnEvent(const int& event)
        {
            receivedValue = event;
        }

        int receivedValue = -1;
    };

    TEST_F(EventStreamTest, ItProcessesAnEvent)
    {
        const auto handler = std::make_shared<MockHandler>();
        stream.AddHandler(handler, UKControllerPlugin::EventHandler::EventHandlerFlags::Sync);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventStreamTest, ItProcessesAnEventAsync)
    {
        const auto handler = std::make_shared<MockHandler>();
        stream.AddHandler(handler, UKControllerPlugin::EventHandler::EventHandlerFlags::Async);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventStreamTest, ItProcessesAnEventToMultipleHandlers)
    {
        const auto handler1 = std::make_shared<MockHandler>();
        const auto handler2 = std::make_shared<MockHandler>();
        stream.AddHandler(handler1, UKControllerPlugin::EventHandler::EventHandlerFlags::Sync);
        stream.AddHandler(handler2, UKControllerPlugin::EventHandler::EventHandlerFlags::Sync);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler1->receivedValue);
        EXPECT_EQ(123, handler2->receivedValue);
    }
} // namespace UKControllerPluginTest::EventHandler
