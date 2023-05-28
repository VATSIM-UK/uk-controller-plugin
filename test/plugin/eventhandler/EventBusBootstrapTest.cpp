#include "bootstrap/PersistenceContainer.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventBusBootstrap.h"
#include "eventhandler/EventHandler.h"
#include "eventhandler/MutableEventBus.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPluginTest::EventHandler {

    class TestEventBusHandler : public UKControllerPluginUtils::EventHandler::EventHandler<int>
    {
        public:
        void OnEvent(const int& event) override
        {
            receivedEvent = event;
        }

        int receivedEvent = 0;
    };

    class EventBusBootstrapTest : public testing::Test
    {
        public:
        EventBusBootstrapTest()
        {
            container.timedHandler = std::make_unique<UKControllerPlugin::TimedEvent::TimedEventCollection>();
        }

        UKControllerPlugin::Bootstrap::PersistenceContainer container;
    };

    TEST_F(EventBusBootstrapTest, BootstrapsEventBus)
    {
        UKControllerPlugin::EventHandler::BootstrapEventBus(container);

        // The bus should be initialised
        EXPECT_TRUE(UKControllerPluginUtils::EventHandler::MutableEventBus::IsInitialised());

        // If the bus is setup, we should be able to register for events
        auto handler = std::make_shared<TestEventBusHandler>();
        UKControllerPluginUtils::EventHandler::EventBus::Bus().AddHandler<int>(
            handler, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);

        // We should also be able to trigger and receive them
        UKControllerPluginUtils::EventHandler::EventBus::Bus().OnEvent(1);
        EXPECT_EQ(1, handler->receivedEvent);

        // And shutting down the event bus should uninitialise it
        UKControllerPlugin::EventHandler::ShutdownEventBus();
        EXPECT_FALSE(UKControllerPluginUtils::EventHandler::MutableEventBus::IsInitialised());
    }

    TEST_F(EventBusBootstrapTest, ItRegistersATimedEventHandlerToProcessEuroscopeThreadEvents)
    {
        UKControllerPlugin::EventHandler::BootstrapEventBus(container);

        // The bus should be initialised
        EXPECT_TRUE(UKControllerPluginUtils::EventHandler::MutableEventBus::IsInitialised());

        // The containers timedHandler should have a single event registered at 1s intervals
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(1));
    }
} // namespace UKControllerPluginTest::EventHandler
