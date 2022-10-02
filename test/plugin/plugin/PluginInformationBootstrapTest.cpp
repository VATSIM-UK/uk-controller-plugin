#include "bootstrap/PersistenceContainer.h"
#include "plugin/PluginInformationBootstrap.h"
#include "plugin/FunctionCallEventHandler.h"
#include "command/CommandHandlerCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Plugin::BootstrapPluginInformationMessage;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;

namespace UKControllerPluginTest::Plugin {

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
        EXPECT_EQ(6, this->configurableDisplays.CountDisplays());
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersCommandHandlers)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_EQ(4, this->container.commandHandlers->CountHandlers());
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersOpenHelpPageCallback)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Plugin Help Page"));
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersOpenDataFolderCallback)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Open Data Folder"));
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersOpenLogsFolderCallback)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Open Logs Folder"));
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersForceUpdateCallback)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Plugin Force Update"));
    }

    TEST_F(PluginInformationBootstrapTest, ItRegistersChangelogCallback)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Open Data Folder"));
    }
    
    TEST_F(PluginInformationBootstrapTest, ItRegistersFunctionCallHandlers)
    {
        BootstrapPluginInformationMessage(this->container, this->configurableDisplays);
        EXPECT_EQ(4, this->container.pluginFunctionHandlers->CountCallbacks());
    }
} // namespace UKControllerPluginTest::Plugin
