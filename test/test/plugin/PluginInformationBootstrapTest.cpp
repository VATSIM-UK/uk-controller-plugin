#include "pch/pch.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/PluginInformationBootstrap.h"
#include "plugin/FunctionCallEventHandler.h"
#include "command/CommandHandlerCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using UKControllerPlugin::Plugin::BootstrapPluginInformationMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Plugin {

        class PluginInformationBootstrapTest : public Test
        {
            public:

                PluginInformationBootstrapTest()
                {
                    container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    container.commandHandlers.reset(new CommandHandlerCollection);
                }

                ConfigurableDisplayCollection configurableDisplays;
                PersistenceContainer container;
        };

        TEST_F(PluginInformationBootstrapTest, ItRegistersConfigurableDisplays)
        {
            BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
            EXPECT_EQ(2, this->configurableDisplays.CountDisplays());
        }

        TEST_F(PluginInformationBootstrapTest, ItRegistersCommandHandlers)
        {
            BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
            EXPECT_EQ(2, this->container.commandHandlers->CountHandlers());
        }

        TEST_F(PluginInformationBootstrapTest, ItRegistersFunctionCallHandlers)
        {
            BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
            EXPECT_EQ(2, this->container.pluginFunctionHandlers->CountCallbacks());
        }
    }  // namespace Plugin
}  // namespace UKControllerPluginTest
