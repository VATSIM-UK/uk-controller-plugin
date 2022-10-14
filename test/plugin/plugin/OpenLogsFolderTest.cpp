#include "data/PluginDataLocations.h"
#include "plugin/OpenLogsFolder.h"
#include "plugin/PopupMenuItem.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Plugin::OpenLogsFolder;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::Plugin {

    class OpenLogsFolderTest : public Test
    {
        public:
        OpenLogsFolderTest() : page(mockWindows, 123)
        {
        }

        NiceMock<MockWinApi> mockWindows;
        OpenLogsFolder page;
    };

    TEST_F(OpenLogsFolderTest, ItHasAConfigurationMenuItem)
    {
        PopupMenuItem expected;
        expected.firstValue = "Plugin Logs Folder (Opens In Explorer)";
        expected.secondValue = "";
        expected.callbackFunctionId = 123;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expected.disabled = false;
        expected.fixedPosition = false;
        EXPECT_EQ(expected, this->page.GetConfigurationMenuItem());
    }

    TEST_F(OpenLogsFolderTest, ConfigureOpensExplorer)
    {
        EXPECT_CALL(this->mockWindows, OpenExplorer(GetFullPluginDataRoot() + L"/logs")).Times(1);

        this->page.Configure(123, "", {});
    }
} // namespace UKControllerPluginTest::Plugin
