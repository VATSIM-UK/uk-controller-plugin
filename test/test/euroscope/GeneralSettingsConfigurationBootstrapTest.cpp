#include "pch/pch.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/UserSetting.h"
#include "mock/MockWinApi.h"
#include "mock/MockUserSettingProviderInterface.h"

using UKControllerPlugin::Euroscope::GeneralSettingsConfigurationBootstrap;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

using ::testing::Test;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationBootstrapTest : public Test
        {
            public:

                GeneralSettingsConfigurationBootstrapTest () :
                    userSettings(mockUserSettingProvider)
                {

                }

                FunctionCallEventHandler functionHandler;
                ConfigurableDisplayCollection configurableDisplays;
                CommandHandlerCollection commandHandlers;
                UserSetting userSettings;
                StrictMock<MockWinApi> windowsApi;
                StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        };


        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersConfigurationCallback)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->userSettings,
                this->windowsApi
            );

            EXPECT_EQ(1, this->functionHandler.CountCallbacks());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheConfigurationMenu)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->userSettings,
                this->windowsApi
            );

            EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        }

        TEST_F(GeneralSettingsConfigurationBootstrapTest, BootstrapRadarScreenRegistersInTheCommandHandlers)
        {
            GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
                this->functionHandler,
                this->configurableDisplays,
                this->commandHandlers,
                this->userSettings,
                this->windowsApi
            );

            EXPECT_EQ(1, this->commandHandlers.CountHandlers());
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
