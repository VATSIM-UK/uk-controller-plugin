#include "pch/pch.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/UserSetting.h"
#include "mock/MockWinApi.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;

using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationBootstrapTest : public Test
        {
            public:

                GeneralSettingsConfigurationBootstrapTest () :
                    userSettings(mockUserSettingProvider), dialogManager(mockDialogProvider)
                {

                }

                FunctionCallEventHandler functionHandler;
                ConfigurableDisplayCollection configurableDisplays;
                CommandHandlerCollection commandHandlers;
                UserSettingAwareCollection userSettingCollection;
                UserSetting userSettings;
                NiceMock<MockDialogProvider> mockDialogProvider;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                DialogManager dialogManager;
        };

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersConfigurationCallback)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->dialogManager
            );

            EXPECT_EQ(1, this->functionHandler.CountCallbacks());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheConfigurationMenu)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->dialogManager
            );

            EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheCommandHandlers)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->dialogManager
            );

            EXPECT_EQ(1, this->commandHandlers.CountHandlers());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapPluginAddsDialogToDialogManager)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapPlugin(
                this->dialogManager,
                this->userSettings,
                this->userSettingCollection
            );

            EXPECT_EQ(1, this->dialogManager.CountDialogs());
            EXPECT_TRUE(this->dialogManager.HasDialog(IDD_GENERAL_SETTINGS));
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
