#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "integration/ExternalMessageEventHandler.h"
#include "integration/InitialisationSuccessMessage.h"
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationModule.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Integration::BootstrapPlugin;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;
using UKControllerPlugin::Integration::InitialisationSuccessMessage;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::Integration {

    class IntegrationModuleTest : public Test
    {
        public:
        IntegrationModuleTest()
        {
            container.externalEventHandler = std::make_shared<ExternalMessageEventHandler>(true);
            container.commandHandlers = std::make_unique<CommandHandlerCollection>();
            container.timedHandler = std::make_unique<TimedEventCollection>();
            testEvent = std::make_shared<InitialisationSuccessMessage>("foo");
        }

        std::shared_ptr<InitialisationSuccessMessage> testEvent;
        PersistenceContainer container;
    };

    TEST_F(IntegrationModuleTest, ItDoesNothingIfDuplicatePlugin)
    {
        BootstrapPlugin(container, true, true);
        EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
        EXPECT_EQ(0, container.commandHandlers->CountHandlers());
    }

    TEST_F(IntegrationModuleTest, ItDoesNothingIfWinsockNotInitialised)
    {
        BootstrapPlugin(container, false, false);
        EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
        EXPECT_EQ(0, container.commandHandlers->CountHandlers());
    }

    TEST_F(IntegrationModuleTest, ItSetsUpIntegrationMessageProcessorsRegardlessOfDuplicatePlugin)
    {
        BootstrapPlugin(container, true, true);
        EXPECT_EQ(0, container.integrationModuleContainer->inboundMessageHandler->CountProcessors());
    }

    TEST_F(IntegrationModuleTest, ItSetsUpOutboundHandlerRegardlessOfDuplicatePlugin)
    {
        BootstrapPlugin(container, true, true);
        EXPECT_NO_THROW(container.integrationModuleContainer->outboundMessageHandler->SendEvent(testEvent));
    }

    TEST_F(IntegrationModuleTest, ItSetsUpExternalEventHandlerRegardlessOfDuplicatePlugin)
    {
        BootstrapPlugin(container, true, true);
        EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
    }

    TEST_F(IntegrationModuleTest, ItSetsUpIntegrationMessageProcessorsRegardlessOfWinsockInitialisation)
    {
        BootstrapPlugin(container, false, false);
        EXPECT_EQ(0, container.integrationModuleContainer->inboundMessageHandler->CountProcessors());
    }

    TEST_F(IntegrationModuleTest, ItSetsUpOutboundHandlerRegardlessOfWinsockIntegration)
    {
        BootstrapPlugin(container, false, false);
        EXPECT_NO_THROW(container.integrationModuleContainer->outboundMessageHandler->SendEvent(testEvent));
    }

    TEST_F(IntegrationModuleTest, ItSetsUpExternalEventHandlerRegardlessOfWinsockInitialisation)
    {
        BootstrapPlugin(container, false, false);
        EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
    }

    TEST_F(IntegrationModuleTest, ItRegistersForCommands)
    {
        BootstrapPlugin(container, false, true);
        EXPECT_EQ(1, container.commandHandlers->CountHandlers());
    }

    TEST_F(IntegrationModuleTest, ItRegistersForNonTimeCriticalTimeEvents)
    {
        BootstrapPlugin(container, false, true);
        EXPECT_EQ(4, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(5));
    }

    TEST_F(IntegrationModuleTest, ItRegistersForTimeCriticalTimeEvents)
    {
        BootstrapPlugin(container, false, true);
        EXPECT_EQ(4, container.timedHandler->CountHandlers());
        EXPECT_EQ(3, container.timedHandler->CountHandlersForFrequency(1));
    }

    TEST_F(IntegrationModuleTest, ItSetsUpOutboundHandler)
    {
        BootstrapPlugin(container, false, true);
        EXPECT_NO_THROW(container.integrationModuleContainer->outboundMessageHandler->SendEvent(testEvent));
    }
} // namespace UKControllerPluginTest::Integration
