#include "MockEuroscopeThreadEventSink.h"
#include "eventhandler/EventHandler.h"
#include "eventhandler/EventStream.h"
#include "test/EventBusTestCase.h"

using UKControllerPluginUtils::EventHandler::EventStream;

namespace UKControllerPluginUtilsTest::EventHandler {
    class EventStreamTest : public testing::Test
    {
        public:
        EventStreamTest() : mockProcessor(std::make_shared<MockEuroscopeThreadEventSink>()), stream(mockProcessor)
        {
        }

        std::shared_ptr<MockEuroscopeThreadEventSink> mockProcessor;
        EventStream<int> stream;
    };

    class MockHandler : public UKControllerPluginUtils::EventHandler::EventHandler<int>
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
        stream.AddHandler(handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventStreamTest, ItProcessesAnEventAsync)
    {
        const auto handler = std::make_shared<MockHandler>();
        stream.AddHandler(handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Async);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventStreamTest, ItProcessesAnEventOnTheEuroscopeThread)
    {
        EXPECT_CALL(*mockProcessor, OnEvent(testing::_))
            .Times(1)
            .WillOnce(testing::Invoke([](const std::function<void()>& event) { event(); }));

        const auto handler = std::make_shared<MockHandler>();
        stream.AddHandler(handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::EuroscopeThread);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventStreamTest, ItProcessesAnEventToMultipleHandlers)
    {
        const auto handler1 = std::make_shared<MockHandler>();
        const auto handler2 = std::make_shared<MockHandler>();
        stream.AddHandler(handler1, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        stream.AddHandler(handler2, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        stream.OnEvent(123);
        EXPECT_EQ(123, handler1->receivedValue);
        EXPECT_EQ(123, handler2->receivedValue);
    }
} // namespace UKControllerPluginUtilsTest::EventHandler
