#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandler.h"
#include "test/EventBusTestCase.h"

using UKControllerPlugin::EventHandler::EventBus;

namespace UKControllerPluginTest::EventHandler {
    class EventBusTest : public EventBusTestCase
    {
        public:
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

    TEST_F(EventBusTest, ItProcessesAnEvent)
    {
        const auto handler = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(handler, UKControllerPlugin::EventHandler::EventHandlerFlags::Sync);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventBusTest, ItProcessesAnEventAsync)
    {
        const auto handler = std::make_shared<MockHandler>();
        EventBus::Bus().AddHandler<int>(handler, UKControllerPlugin::EventHandler::EventHandlerFlags::Async);
        EventBus::Bus().OnEvent(123);
        EXPECT_EQ(123, handler->receivedValue);
    }

    TEST_F(EventBusTest, ItSendsAnEventForObservation)
    {
        EventBus::Bus().OnEvent(123);

        EXPECT_EQ(1, EventBusObserver().observedEvents.size());
        EXPECT_EQ(123, std::any_cast<int>(EventBusObserver().observedEvents[0]));
    }
} // namespace UKControllerPluginTest::EventHandler
