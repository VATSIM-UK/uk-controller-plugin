#include "pch/pch.h"
#include "integration/IntegrationModule.h"
#include "integration/ExternalMessageEventHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::Test;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Integration::BootstrapPlugin;

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
                }

                PersistenceContainer container;
        };

        TEST_F(IntegrationModuleTest, ItDoesNothingIfDuplicatePlugin)
        {
            BootstrapPlugin(container, true);
            EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
            EXPECT_EQ(0, container.commandHandlers->CountHandlers());
            EXPECT_EQ(0, container.timedHandler->CountHandlers());
        }

        TEST_F(IntegrationModuleTest, ItSetsUpExternalEventHandler)
        {
            BootstrapPlugin(container, true);
            EXPECT_EQ(0, container.externalEventHandler->CountHandlers());
        }

        TEST_F(IntegrationModuleTest, ItRegistersForCommands)
        {
            BootstrapPlugin(container, false);
            EXPECT_EQ(1, container.commandHandlers->CountHandlers());
        }

        TEST_F(IntegrationModuleTest, ItRegistersForTimedEventsEverySecond)
        {
            BootstrapPlugin(container, false);
            EXPECT_EQ(1, container.timedHandler->CountHandlers());
            EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(1));
        }
    }  // namespace Integration
}  // namespace UKControllerPluginTest
