#include "pch/pch.h"
#include "notifications/NotificationsModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockApiInterface.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using testing::Test;
using testing::NiceMock;
using UKControllerPlugin::Notifications::BootstrapPlugin;
using UKControllerPlugin::Notifications::BootstrapRadarScreen;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationsModuleTest : public Test
        {
            public:
                NotificationsModuleTest()
                {
                    container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    container.api.reset(new NiceMock<Api::MockApiInterface>);
                    container.dialogManager.reset(new DialogManager(dialogProvider));
                }

                ConfigurableDisplayCollection configurableDisplays;
                NiceMock<MockDialogProvider> dialogProvider;
                PersistenceContainer container;
        };

        TEST_F(NotificationsModuleTest, ItRegistersTheNotificationsDialog)
        {
            BootstrapPlugin(container);
            EXPECT_TRUE(container.dialogManager->HasDialog(IDD_NOTIFICATIONS));
        }

        TEST_F(NotificationsModuleTest, ItRegistersAllDialogs)
        {
            BootstrapPlugin(container);
            EXPECT_EQ(1, container.dialogManager->CountDialogs());
        }

        TEST_F(NotificationsModuleTest, ItSetsUpTheCallback)
        {
            BootstrapPlugin(container);
            EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(NotificationsModuleTest, ItRegistersWithTheConfigurableDisplays)
        {
            BootstrapPlugin(container);
            BootstrapRadarScreen(configurableDisplays);
            EXPECT_EQ(1, configurableDisplays.CountDisplays());
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
