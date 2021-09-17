#include "euroscope/GeneralSettingsConfiguration.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::GeneralSettingsConfiguration;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationTest : public Test
        {
            public:
            GeneralSettingsConfigurationTest() : dialogManager(dialogProvider), configuration(this->dialogManager, 1)
            {
                this->dialogManager.AddDialog(generalSettingsData);
            }

            DialogData generalSettingsData = {IDD_GENERAL_SETTINGS, "Test"};
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
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->generalSettingsData, _)).Times(1);

            this->configuration.ProcessCommand(".ukcp g");
        }

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandReturnsTrueOnSuccess)
        {
            EXPECT_TRUE(this->configuration.ProcessCommand(".ukcp g"));
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasADescription)
        {
            EXPECT_EQ(
                this->configuration.configMenuDescription, this->configuration.GetConfigurationMenuItem().firstValue);
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasTheCorrectCallbackId)
        {
            EXPECT_EQ(
                this->configuration.menuSelectedCallbackId,
                this->configuration.GetConfigurationMenuItem().callbackFunctionId);
        }

        TEST_F(GeneralSettingsConfigurationTest, GetPopupMenuItemHasNoCheckbox)
        {
            EXPECT_EQ(
                EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX, this->configuration.GetConfigurationMenuItem().checked);
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
            EXPECT_CALL(this->dialogProvider, OpenDialog(this->generalSettingsData, _)).Times(1);

            this->configuration.Configure(0, "test", {});
        }
    } // namespace Euroscope
} // namespace UKControllerPluginTest
