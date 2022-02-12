#include "aircraft/CallsignSelectionList.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "list/PopupList.h"
#include "list/PopupListFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Aircraft::CallsignSelectionList;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::Aircraft {
    class CallsignSelectionListFactoryTest : public testing::Test
    {
        public:
        CallsignSelectionListFactoryTest()
            : callsignProvider(std::make_shared<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>>()),
              listFactory(functionHandler, plugin), factory(listFactory)
        {
        }

        FunctionCallEventHandler functionHandler;
        std::shared_ptr<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>> callsignProvider;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        PopupListFactory listFactory;
        CallsignSelectionListFactory factory;
    };

    TEST_F(CallsignSelectionListFactoryTest, ItRegistersWithFunctionHandler)
    {
        static_cast<void>(factory.Create(callsignProvider, "Some Function"));
        EXPECT_TRUE(functionHandler.HasCallbackByDescription("Some Function"));
    }

    TEST_F(CallsignSelectionListFactoryTest, CallbackFunctionTriggersCallsignSelected)
    {
        static_cast<void>(factory.Create(callsignProvider, "Some Function"));
        functionHandler.CallFunction(
            functionHandler.ReserveNextDynamicFunctionId() - 1, "foo", flightplan, radarTarget, {}, RECT{});
    }

    TEST_F(CallsignSelectionListFactoryTest, ListIsTriggeredWithCorrectId)
    {
        auto list = factory.Create(callsignProvider, "Some Function");

        EXPECT_CALL(*callsignProvider, GetCallsigns).Times(1).WillOnce(testing::Return(std::set<std::string>{"ABCD"}));

        EXPECT_CALL(plugin, TriggerPopupList(RectEq(RECT{1, 2, 201, 402}), "Select Aircraft", 1)).Times(1);

        PopupMenuItem expectedItem;
        expectedItem.firstValue = "ABCD";
        expectedItem.secondValue = "";
        expectedItem.disabled = false;
        expectedItem.fixedPosition = false;
        expectedItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem.callbackFunctionId = functionHandler.ReserveNextDynamicFunctionId() - 1;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem)).Times(1);

        list->Trigger({1, 2});
    }
} // namespace UKControllerPluginTest::Aircraft
