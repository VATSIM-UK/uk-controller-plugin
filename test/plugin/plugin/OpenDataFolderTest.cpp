#include "data/PluginDataLocations.h"
#include "plugin/OpenDataFolder.h"
#include "plugin/PopupMenuItem.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Plugin::OpenDataFolder;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::Plugin {

    class OpenDataFolderTest : public Test
    {
        public:
        OpenDataFolderTest() : page(mockWindows, 123)
        {
        }

        NiceMock<MockWinApi> mockWindows;
        OpenDataFolder page;
    };

    TEST_F(OpenDataFolderTest, ItHasAConfigurationMenuItem)
    {
        PopupMenuItem expected;
        expected.firstValue = "Plugin Data Folder (Opens In Explorer)";
        expected.secondValue = "";
        expected.callbackFunctionId = 123;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expected.disabled = false;
        expected.fixedPosition = false;
        EXPECT_EQ(expected, this->page.GetConfigurationMenuItem());
    }

    TEST_F(OpenDataFolderTest, ConfigureOpensExplorer)
    {
        EXPECT_CALL(this->mockWindows, OpenExplorer(GetFullPluginDataRoot())).Times(1);

        this->page.Configure(123, "", {});
    }
} // namespace UKControllerPluginTest::Plugin
