#include "pch/pch.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "euroscope/UserSetting.h"
#include "mock/MockWinApi.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Euroscope::GeneralSettingsConfiguration;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Euroscope {

        TEST(GeneralSettingsConfiguration, ProcessCommandReturnsFalseOnIncorrectCommand)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_FALSE(configuration.ProcessCommand(".ukcp h"));
        }

        TEST(GeneralSettingsConfiguration, ProcessCommandOpensDialogOnSuccess)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(windowsApi, OpenGeneralSettingsDialog)
                .Times(1);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            configuration.ProcessCommand(".ukcp g");
        }

        TEST(GeneralSettingsConfiguration, ProcessCommandReturnsTrueOnSuccess)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_TRUE(configuration.ProcessCommand(".ukcp g"));
        }

        TEST(GeneralSettingsConfiguration, GetPopupMenuItemHasADescription)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_EQ(configuration.configMenuDescription, configuration.GetConfigurationMenuItem().firstValue);
        }

        TEST(GeneralSettingsConfiguration, GetPopupMenuItemHasTheCorrectCallbackId)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_EQ(
                configuration.menuSelectedCallbackId,
                configuration.GetConfigurationMenuItem().callbackFunctionId
            );
        }

        TEST(GeneralSettingsConfiguration, GetPopupMenuItemHasNoCheckbox)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_EQ(EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, configuration.GetConfigurationMenuItem().checked);
        }

        TEST(GeneralSettingsConfiguration, GetPopupMenuItemIsEnabled)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_FALSE(configuration.GetConfigurationMenuItem().disabled);
        }

        TEST(GeneralSettingsConfiguration, GetPopupMenuItemDoesNotHaveFixedPosition)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            EXPECT_FALSE(configuration.GetConfigurationMenuItem().fixedPosition);
        }

        TEST(GeneralSettingsConfiguration, SelectingTheMenuItemOpensConfigurationDialog)
        {
            NiceMock<MockWinApi> windowsApi;
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(windowsApi, OpenGeneralSettingsDialog)
                .Times(1);

            GeneralSettingsConfiguration configuration(userSetting, windowsApi, 1);
            configuration.Configure("test");
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
