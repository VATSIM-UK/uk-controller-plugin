#include "pch/pch.h"
#include "integration/IntegrationModule.h"
#include "integration/ExternalMessageEventHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "integration/InitialisationSuccessMessage.h"

using ::testing::Test;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Integration::BootstrapPlugin;
using UKControllerPlugin::Integration::InitialisationSuccessMessage;

namespace UKControllerPluginTest {
    namespace Integration {

        class IntegrationModuleTest : public Test
        {
            public:
                IntegrationModuleTest()
                {
                    container.externalEventHandler.reset(new ExternalMessageEventHandler(true));
                    container.commandHandlers.reset(new CommandHandlerCollection);
                    container.timedHandler.reset(new TimedEventCollection);
                    testEvent = std::make_shared<InitialisationSuccessMessage>();
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
    }  // namespace Integration
}  // namespace UKControllerPluginTest
