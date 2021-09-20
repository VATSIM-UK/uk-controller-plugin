#include "plugin/PluginChangelog.h"
#include "plugin/PopupMenuItem.h"
#include "update/LoadChangelog.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Plugin::PluginChangelog;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Plugin {

        class PluginChangelogTest : public Test
        {
            public:
            PluginChangelogTest() : page(mockWindows, 123)
            {
            }

            NiceMock<MockWinApi> mockWindows;
            PluginChangelog page;
        };

        TEST_F(PluginChangelogTest, ItHasAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "UKCP Changelog (Opens In Web Browser)";
            expected.secondValue = "";
            expected.callbackFunctionId = 123;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;
            EXPECT_EQ(expected, this->page.GetConfigurationMenuItem());
        }

        TEST_F(PluginChangelogTest, ProcessCommandReturnsFalseIfNotCommand)
        {
            EXPECT_FALSE(this->page.ProcessCommand(".ukcp notthis"));
        }

        TEST_F(PluginChangelogTest, ProcessCommandReturnsTrueIfCommand)
        {
            EXPECT_TRUE(this->page.ProcessCommand(".ukcp changelog"));
        }

        TEST_F(PluginChangelogTest, ProcessCommandShowsChangelog)
        {
            EXPECT_CALL(this->mockWindows, OpenWebBrowser(UKControllerPlugin::Update::changelogUrl)).Times(1);

            this->page.ProcessCommand(".ukcp changelog");
        }

        TEST_F(PluginChangelogTest, ConfigureShowsChangelog)
        {
            EXPECT_CALL(this->mockWindows, OpenWebBrowser(UKControllerPlugin::Update::changelogUrl)).Times(1);

            this->page.Configure(123, "", {});
        }
    } // namespace Plugin
} // namespace UKControllerPluginTest
