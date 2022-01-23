#include "aircraft/CallsignSelectionList.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Aircraft::CallsignSelectionList;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::Aircraft {
    class CallsignSelectionListFactoryTest : public testing::Test
    {
        public:
        CallsignSelectionListFactoryTest()
            : callsignProvider(std::make_shared<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>>()),
              factory(functionHandler, plugin)
        {
        }

        FunctionCallEventHandler functionHandler;
        std::shared_ptr<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>> callsignProvider;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        CallsignSelectionListFactory factory;
    };

    TEST_F(CallsignSelectionListFactoryTest, ItRegistersWithFunctionHandler)
    {
        static_cast<void>(factory.Create(callsignProvider, "Some Function"));
        EXPECT_TRUE(functionHandler.HasCallbackByDescription("Some Function"));
    }

    TEST_F(CallsignSelectionListFactoryTest, CallbackFunctionTriggersCallsignSelected)
    {
        auto list = factory.Create(callsignProvider, "Some Function");

        EXPECT_CALL(*callsignProvider, CallsignSelected("BAW123")).Times(1);

        list->CallsignSelected("BAW123");
    }

    TEST_F(CallsignSelectionListFactoryTest, ListIsTriggeredWithCorrectId)
    {
        auto list = factory.Create(callsignProvider, "Some Function");

        EXPECT_CALL(*callsignProvider, GetCallsigns).Times(1).WillOnce(testing::Return(std::set<std::string>{"ABCD"}));

        EXPECT_CALL(plugin, TriggerPopupList(RectEq(RECT{1, 2, 51, 102}), "Select Aircraft", 1)).Times(1);

        PopupMenuItem expectedItem;
        expectedItem.firstValue = "ABCD";
        expectedItem.secondValue = "";
        expectedItem.disabled = false;
        expectedItem.fixedPosition = false;
        expectedItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem.callbackFunctionId = functionHandler.ReserveNextDynamicFunctionId() - 1;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem)).Times(1);

        list->TriggerList({1, 2});
    }
} // namespace UKControllerPluginTest::Aircraft
