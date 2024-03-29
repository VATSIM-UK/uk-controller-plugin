#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandler.h"
#include "test/EventBusTestCase.h"

using UKControllerPluginUtils::EventHandler::EventBus;

namespace UKControllerPluginUtilsTest::EventHandler {
    class EventBusTest : public EventBusTestCase
    {
        public:
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

    TEST_F(EventBusTest, ItProcessesAnEvent)
    {
        const auto handler = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventBusTest, ItProcessesAnEventAsync)
    {
        const auto handler = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Async);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventBusTest, ItProcessesAnEventOnTheEuroscopeThread)
    {
        const auto handler = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(
            handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::EuroscopeThread);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventBusTest, ItProcessesAnEventToMultipleHandlers)
    {
        const auto handler1 = std::make_shared<MockHandler>();
        const auto handler2 = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(handler1, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        EventBus::Bus().AddHandler<int>(handler2, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler1->receivedValue);
        EXPECT_EQ(123, handler2->receivedValue);
    }

    TEST_F(EventBusTest, ItSendsAnEventForObservation)
    {
        EventBus::Bus().OnEvent(123);

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(123, std::any_cast<int>(EventBusObserver().observedEvents[0]));
    }
} // namespace UKControllerPluginUtilsTest::EventHandler
