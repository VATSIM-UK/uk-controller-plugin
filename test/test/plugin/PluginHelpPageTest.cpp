#include "pch/pch.h"
#include "plugin/PluginHelpPage.h"
#include "plugin/PopupMenuItem.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Plugin::PluginHelpPage;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Plugin {

        class PluginHelpPageTest : public Test
        {
            public:

                PluginHelpPageTest()
                    : page(mockWindows, 123)
                {

                }

                NiceMock<MockWinApi> mockWindows;
                PluginHelpPage page;
        };

        TEST_F(PluginHelpPageTest, ItHasAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "UKCP Help (Opens In Web Browser)";
            expected.secondValue = "";
            expected.callbackFunctionId = 123;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;
            EXPECT_EQ(expected, this->page.GetConfigurationMenuItem());
        }

        TEST_F(PluginHelpPageTest, ProcessCommandReturnsFalseIfNotCommand)
        {
            EXPECT_FALSE(this->page.ProcessCommand(".ukcp notthis"));
        }

        TEST_F(PluginHelpPageTest, ProcessCommandReturnsTrueIfCommand)
        {
            EXPECT_TRUE(this->page.ProcessCommand(".ukcp help"));
        }

        TEST_F(PluginHelpPageTest, ProcessCommandShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenWebBrowser(page.helpUrl))
                .Times(1);

            this->page.ProcessCommand(".ukcp help");
        }

        TEST_F(PluginHelpPageTest, ConfigureShowsMessage)
        {
            EXPECT_CALL(this->mockWindows, OpenWebBrowser(std::wstring(page.helpUrl)))
                .Times(1);

            this->page.Configure(123, "", {});
        }
    }  // namespace Plugin
}  // namespace UKControllerPluginTest
