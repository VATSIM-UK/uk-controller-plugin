#include "pch/pch.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "mock/MockDialogProvider.h"
#include "plugin/PopupMenuItem.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"

using UKControllerPlugin::Euroscope::GeneralSettingsConfiguration;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Dialog::DialogData;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationTest : public Test
        {
            public:

                GeneralSettingsConfigurationTest()
                    : configuration(this->dialogManager, 1), dialogManager(dialogProvider)
                {
                    this->dialogManager.AddDialog(generalSettingsData);
                }

                DialogData generalSettingsData = { IDD_GENERAL_SETTINGS, "Test" };
                NiceMock<MockDialogProvider> dialogProvider;
                DialogManager dialogManager;
                GeneralSettingsConfiguration configuration;
        };

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandReturnsFalseOnIncorrectCommand)
        {
            EXPECT_FALSE(this->configuration.ProcessCommand(".ukcp h"));
        }

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandOpensDialogOnSuccess)
        {
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->generalSettingsData, _))
                .Times(1);

            this->configuration.ProcessCommand(".ukcp g");
        }

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandReturnsTrueOnSuccess)
        {
            EXPECT_TRUE(this->configuration.ProcessCommand(".ukcp g"));
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasADescription)
        {
            EXPECT_EQ(
                this->configuration.configMenuDescription,
                this->configuration.GetConfigurationMenuItem().firstValue
            );
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasTheCorrectCallbackId)
        {
            EXPECT_EQ(
                this->configuration.menuSelectedCallbackId,
                this->configuration.GetConfigurationMenuItem().callbackFunctionId
            );
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasNoCheckbox)
        {
            EXPECT_EQ(
                EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX,
                this->configuration.GetConfigurationMenuItem().checked
            );
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemIsEnabled)
        {
            EXPECT_FALSE(this->configuration.GetConfigurationMenuItem().disabled);
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemDoesNotHaveFixedPosition)
        {
            EXPECT_FALSE(this->configuration.GetConfigurationMenuItem().fixedPosition);
        }

        TEST_F(GeneralSettingsConfigurationTest, SelectingTheMenuItemOpensConfigurationDialog)
        {
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->generalSettingsData, _))
                .Times(1);

            this->configuration.Configure(0, "test", {});
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
