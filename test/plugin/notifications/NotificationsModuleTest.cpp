#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "dialog/DialogManager.h"
#include "notifications/NotificationsModule.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Notifications::BootstrapPlugin;
using UKControllerPlugin::Notifications::BootstrapRadarScreen;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest::Notifications {

    class NotificationsModuleTest : public Test
    {
        public:
        NotificationsModuleTest()
        {
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            container.api = std::make_unique<NiceMock<Api::MockApiInterface>>();
            container.dialogManager = std::make_unique<DialogManager>(dialogProvider);
            container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
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

    TEST_F(NotificationsModuleTest, ItRegistersForActiveCallsignEvents)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.activeCallsigns->CountHandlers());
    }

    TEST_F(NotificationsModuleTest, ItRegistersWithConfigurableDisplaysOnRadarScreens)
    {
        BootstrapPlugin(container);
        BootstrapRadarScreen(container, configurableDisplays);
        EXPECT_EQ(1, configurableDisplays.CountDisplays());
    }
} // namespace UKControllerPluginTest::Notifications
