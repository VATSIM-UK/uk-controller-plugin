#include "aircraft/CallsignSelectionList.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Aircraft::CallsignSelectionList;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::Aircraft {
    class CallsignSelectionListTest : public testing::Test
    {
        public:
        CallsignSelectionListTest()
            : callsignProvider(std::make_shared<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>>()),
              list(callsignProvider, plugin, 123)
        {
        }
        std::shared_ptr<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>> callsignProvider;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        CallsignSelectionList list;
    };

    TEST_F(CallsignSelectionListTest, ItDoesntTriggerListIfNoCallsigns)
    {
        EXPECT_CALL(*callsignProvider, GetCallsigns).Times(1).WillOnce(testing::Return(std::set<std::string>()));

        EXPECT_CALL(plugin, TriggerPopupList).Times(0);

        list.TriggerList({1, 2});
    }

    TEST_F(CallsignSelectionListTest, ItDisplaysCallsignList)
    {
        EXPECT_CALL(*callsignProvider, GetCallsigns)
            .Times(1)
            .WillOnce(testing::Return(std::set<std::string>{"ABCD", "EFGH", "IJKL"}));

        EXPECT_CALL(plugin, TriggerPopupList(RectEq(RECT{1, 2, 51, 102}), "Select Aircraft", 1)).Times(1);

        PopupMenuItem expectedItem;
        expectedItem.firstValue = "ABCD";
        expectedItem.secondValue = "";
        expectedItem.disabled = false;
        expectedItem.fixedPosition = false;
        expectedItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem.callbackFunctionId = 123;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem)).Times(1);

        expectedItem.firstValue = "EFGH";

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem)).Times(1);

        expectedItem.firstValue = "IJKL";

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem)).Times(1);

        list.TriggerList({1, 2});
    }

    TEST_F(CallsignSelectionListTest, ItTriggersCallsignSelected)
    {
        EXPECT_CALL(*callsignProvider, CallsignSelected("BAW123")).Times(1);

        list.CallsignSelected("BAW123");
    }
} // namespace UKControllerPluginTest::Aircraft
