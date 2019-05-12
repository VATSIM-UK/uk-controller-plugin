#include "pch/pch.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "mock/MockWinApi.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Euroscope::GeneralSettingsConfiguration;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class GeneralSettingsConfigurationTest : public Test
        {
            public:

                GeneralSettingsConfigurationTest()
                    : configuration(this->winApi, 1)
                {

                }

                NiceMock<MockWinApi> winApi;
                GeneralSettingsConfiguration configuration;
        };

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandReturnsFalseOnIncorrectCommand)
        {
            EXPECT_FALSE(this->configuration.ProcessCommand(".ukcp h"));
        }

        TEST_F(GeneralSettingsConfigurationTest, ProcessCommandOpensDialogOnSuccess)
        {
            EXPECT_CALL(this->winApi, OpenGeneralSettingsDialog)
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
            EXPECT_CALL(this->winApi, OpenGeneralSettingsDialog)
                .Times(1);
            this->configuration.Configure(0, "test", {});
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
