#include "pch/pch.h"
#include "srd/SrdModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "mock/MockDialogProvider.h"

using UKControllerPlugin::Srd::BootstrapPlugin;
using UKControllerPlugin::Srd::BootstrapRadarScreen;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Srd {

        class SrdModuleTest : public Test
        {
            public:

                SrdModuleTest()
                {
                    container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                    container.dialogManager = std::make_unique<DialogManager>(NiceMock<MockDialogProvider>());
                }

                ConfigurableDisplayCollection displays;
                PersistenceContainer container;
        };


        TEST_F(SrdModuleTest, BootstrapPluginRegistersDialog)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_SRD_SEARCH));
        }

        TEST_F(SrdModuleTest, BootstrapPluginRegistersTagFunction)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9004));
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountTagFunctions());
        }

        TEST_F(SrdModuleTest, BootstrapPluginRegistersConfigureFunction)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(SrdModuleTest, BootstrapRadarScreenAddsToConfigurables)
        {
            BootstrapPlugin(this->container);
            BootstrapRadarScreen(this->displays);
            EXPECT_EQ(1, this->displays.CountDisplays());
        }
    }  // namespace Srd
}  // namespace UKControllerPluginTest
