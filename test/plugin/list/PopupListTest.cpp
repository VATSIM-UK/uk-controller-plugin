#include "list/ListItem.h"
#include "list/PopupList.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::List::ListItem;
using UKControllerPlugin::List::ListItemCheckedStatus;
using UKControllerPlugin::List::PopupList;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::List {
    class PopupListTest : public testing::Test
    {
        public:
        PopupListTest()
            : mockProvider(std::make_shared<testing::NiceMock<MockListItemProvider>>()), list(mockProvider, plugin, 55)
        {
        }
        std::shared_ptr<testing::NiceMock<MockListItemProvider>> mockProvider;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        PopupList list;
    };

    TEST_F(PopupListTest, ItDoesntTriggerNoItems)
    {
        EXPECT_CALL(*mockProvider, ListItems)
            .Times(1)
            .WillOnce(testing::Return(std::list<std::shared_ptr<ListItem>>()));
        EXPECT_CALL(plugin, TriggerPopupList(testing::_, testing::_, testing::_)).Times(0);

        list.Trigger({1, 2});
    }

    TEST_F(PopupListTest, ItTriggersList)
    {
        std::list<std::shared_ptr<ListItem>> items;
        items.push_back(
            std::make_shared<ListItem>("1first", "1second", true, false, ListItemCheckedStatus::NoCheckbox));
        items.push_back(
            std::make_shared<ListItem>("2first", "2second", false, true, ListItemCheckedStatus::NotChecked));
        items.push_back(std::make_shared<ListItem>("3first", "3second", false, false, ListItemCheckedStatus::Checked));

        EXPECT_CALL(*mockProvider, ListColumns).Times(1).WillOnce(testing::Return(2));

        EXPECT_CALL(*mockProvider, ListName).Times(1).WillOnce(testing::Return("Test list"));

        EXPECT_CALL(*mockProvider, ListItems).Times(1).WillOnce(testing::Return(items));

        EXPECT_CALL(plugin, TriggerPopupList(RectEq(RECT{1, 2, 201, 402}), "Test list", 2)).Times(1);

        PopupMenuItem expectedItem1;
        expectedItem1.firstValue = "1first";
        expectedItem1.secondValue = "1second";
        expectedItem1.disabled = true;
        expectedItem1.fixedPosition = false;
        expectedItem1.callbackFunctionId = 55;
        expectedItem1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;

        PopupMenuItem expectedItem2;
        expectedItem2.firstValue = "2first";
        expectedItem2.secondValue = "2second";
        expectedItem2.disabled = false;
        expectedItem2.fixedPosition = true;
        expectedItem2.callbackFunctionId = 55;
        expectedItem2.checked = EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;

        PopupMenuItem expectedItem3;
        expectedItem3.firstValue = "3first";
        expectedItem3.secondValue = "3second";
        expectedItem3.disabled = false;
        expectedItem3.fixedPosition = false;
        expectedItem3.callbackFunctionId = 55;
        expectedItem3.checked = EuroScopePlugIn::POPUP_ELEMENT_CHECKED;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem1)).Times(1);

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem2)).Times(1);

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem3)).Times(1);

        list.Trigger({1, 2});
    }
} // namespace UKControllerPluginTest::List
