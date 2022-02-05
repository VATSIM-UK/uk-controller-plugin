#include "list/ListItem.h"
#include "list/PopupList.h"
#include "list/PopupListFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::List::ListItem;
using UKControllerPlugin::List::ListItemCheckedStatus;
using UKControllerPlugin::List::PopupList;
using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::List {
    class PopupListFactoryTest : public testing::Test
    {
        public:
        PopupListFactoryTest()
            : itemProvider(std::make_shared<testing::NiceMock<MockListItemProvider>>()),
              factory(functionHandler, plugin)
        {
        }

        FunctionCallEventHandler functionHandler;
        std::shared_ptr<testing::NiceMock<MockListItemProvider>> itemProvider;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        PopupListFactory factory;
    };

    TEST_F(PopupListFactoryTest, ItRegistersWithFunctionHandler)
    {
        static_cast<void>(factory.Create(itemProvider, "Some Function"));
        EXPECT_TRUE(functionHandler.HasCallbackByDescription("Some Function"));
    }

    TEST_F(PopupListFactoryTest, CallbackFunctionTriggersItemSelected)
    {
        static_cast<void>(factory.Create(itemProvider, "Some Function"));
        EXPECT_CALL(*itemProvider, ItemSelected("foo")).Times(1);

        functionHandler.CallFunction(
            functionHandler.ReserveNextDynamicFunctionId() - 1, "foo", flightplan, radarTarget, {}, RECT{});
    }

    TEST_F(PopupListFactoryTest, ListIsTriggeredWithCorrectId)
    {
        auto list = factory.Create(itemProvider, "Some Function");

        std::list<std::shared_ptr<ListItem>> items;
        items.push_back(
            std::make_shared<ListItem>("1first", "1second", true, false, ListItemCheckedStatus::NoCheckbox));

        EXPECT_CALL(*itemProvider, ListColumns).Times(1).WillOnce(testing::Return(2));

        EXPECT_CALL(*itemProvider, ListName).Times(1).WillOnce(testing::Return("Test list"));

        EXPECT_CALL(*itemProvider, ListItems).Times(1).WillOnce(testing::Return(items));

        EXPECT_CALL(plugin, TriggerPopupList(RectEq(RECT{1, 2, 201, 402}), "Test list", 2)).Times(1);

        PopupMenuItem expectedItem1;
        expectedItem1.firstValue = "1first";
        expectedItem1.secondValue = "1second";
        expectedItem1.disabled = true;
        expectedItem1.fixedPosition = false;
        expectedItem1.callbackFunctionId = functionHandler.ReserveNextDynamicFunctionId() - 1;
        expectedItem1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem1)).Times(1);

        list->Trigger({1, 2});
    }
} // namespace UKControllerPluginTest::List
