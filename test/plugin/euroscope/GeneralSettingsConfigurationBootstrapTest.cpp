#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "theme/ThemeSettings.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Theme::ThemeSettings;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationBootstrapTest : public Test
        {
            public:
            GeneralSettingsConfigurationBootstrapTest() = default;

            void BootstrapContainer()
            {
                container.windows = std::make_unique<NiceMock<Windows::MockWinApi>>();
                container.pluginUserSettingHandler = std::make_unique<UserSetting>(mockUserSettingProvider);
                container.dialogManager = std::make_unique<DialogManager>(mockDialogProvider);
                container.settingsRepository = std::make_unique<SettingRepository>();
                container.commandHandlers = std::make_unique<CommandHandlerCollection>();
                container.userSettingHandlers = std::make_shared<UserSettingAwareCollection>();
                container.themeSettings = std::make_shared<ThemeSettings>(*container.pluginUserSettingHandler);
                container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            }

            PersistenceContainer container;
            ConfigurableDisplayCollection configurableDisplays;
            NiceMock<MockDialogProvider> mockDialogProvider;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        };

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersConfigurationCallback)
        {
            BootstrapContainer();
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                *container.pluginFunctionHandlers,
                configurableDisplays,
                *container.commandHandlers,
                *container.dialogManager);

            EXPECT_EQ(1, container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheConfigurationMenu)
        {
            BootstrapContainer();
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                *container.pluginFunctionHandlers,
                configurableDisplays,
                *container.commandHandlers,
                *container.dialogManager);

            EXPECT_EQ(1, configurableDisplays.CountDisplays());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheCommandHandlers)
        {
            BootstrapContainer();
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                *container.pluginFunctionHandlers,
                configurableDisplays,
                *container.commandHandlers,
                *container.dialogManager);

            EXPECT_EQ(1, container.commandHandlers->CountHandlers());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapPluginAddsDialogToDialogManager)
        {
            BootstrapContainer();
            GeneralSettingsConfigurationBootstrap::BootstrapPlugin(container);

            EXPECT_EQ(1, container.dialogManager->CountDialogs());
            EXPECT_TRUE(container.dialogManager->HasDialog(IDD_GENERAL_SETTINGS));
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
