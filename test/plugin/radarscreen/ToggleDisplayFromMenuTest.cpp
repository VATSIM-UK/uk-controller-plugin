#include "plugin/PopupMenuItem.h"
#include "radarscreen/ToggleDisplayFromMenu.h"

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::RadarScreen::ToggleDisplayFromMenu;

namespace UKControllerPluginTest::RadarScreen {
    class ToggleDisplayFromMenuTest : public testing::Test
    {
        public:
        ToggleDisplayFromMenuTest()
            : displayMock(std::make_shared<testing::NiceMock<MockMenuToggleableDisplay>>()), menu(displayMock, 55)
        {
        }

        std::shared_ptr<testing::NiceMock<MockMenuToggleableDisplay>> displayMock;
        ToggleDisplayFromMenu menu;
    };

    TEST_F(ToggleDisplayFromMenuTest, ConfigureTogglesMenu)
    {
        EXPECT_CALL(*displayMock, Toggle).Times(1);

        menu.Configure(1, "", {});
    }

    TEST_F(ToggleDisplayFromMenuTest, ItReturnsMenuItemChecked)
    {
        EXPECT_CALL(*displayMock, MenuItem).Times(1).WillOnce(testing::Return("Test Item"));

        EXPECT_CALL(*displayMock, IsVisible).Times(1).WillOnce(testing::Return(true));

        PopupMenuItem expected;
        expected.firstValue = "Test Item";
        expected.fixedPosition = false;
        expected.disabled = false;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_CHECKED;
        expected.callbackFunctionId = 55;

        EXPECT_EQ(expected, menu.GetConfigurationMenuItem());
    }

    TEST_F(ToggleDisplayFromMenuTest, ItReturnsMenuItemNotChecked)
    {
        EXPECT_CALL(*displayMock, MenuItem).Times(1).WillOnce(testing::Return("Test Item"));
        EXPECT_CALL(*displayMock, IsVisible).Times(1).WillOnce(testing::Return(false));

        PopupMenuItem expected;
        expected.firstValue = "Test Item";
        expected.fixedPosition = false;
        expected.disabled = false;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;
        expected.callbackFunctionId = 55;

        EXPECT_EQ(expected, menu.GetConfigurationMenuItem());
    }
} // namespace UKControllerPluginTest::RadarScreen
