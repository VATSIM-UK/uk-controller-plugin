#include "controller/ControllerPosition.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "squawk/SquawkAssignmentMenu.h"
#include "squawk/SquawkGeneratorInterface.h"

namespace UKControllerPluginTest::Squawk {

    class MockSquawkGenerator : public UKControllerPlugin::Squawk::SquawkGeneratorInterface
    {
        public:
        MOCK_METHOD(
            bool,
            DeleteApiSquawkAndSetTo,
            (const std::string&, UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&),
            (override));
        MOCK_METHOD(
            bool,
            ForceGeneralSquawkForAircraft,
            (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
             UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&),
            (override));
        MOCK_METHOD(
            bool,
            ForceLocalSquawkForAircraft,
            (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&,
             UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface&),
            (override));
    };

    class SquawkAssignmentMenuTest : public ::testing::Test
    {
        public:
        SquawkAssignmentMenuTest()
            : mockFlightplan(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              mockRadarTarget(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              menu(55, 66, squawkGenerator, activeCallsigns, plugin)
        {
            ON_CALL(*mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));

            auto controllerPosition = UKControllerPlugin::Controller::ControllerPosition(
                2, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, true

            );
            auto activeUserCallsign =
                UKControllerPlugin::Controller::ActiveCallsign("EGFF_APP", "Test", controllerPosition, true);
            activeCallsigns.AddUserCallsign(activeUserCallsign);
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
        testing::NiceMock<MockSquawkGenerator> squawkGenerator;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        UKControllerPlugin::Squawk::SquawkAssignmentMenu menu;
    };

    TEST_F(SquawkAssignmentMenuTest, ItDoesntTriggerPopupListIfUserDoesNotHaveActiveCallsign)
    {
        activeCallsigns.Flush();
        EXPECT_CALL(plugin, TriggerPopupList).Times(0);

        menu.DisplaySquawkAssignmentMenu(*mockFlightplan, {0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, ItDoesntTriggerPopupListIfAircraftTrackedBySomeoneElse)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(false));

        EXPECT_CALL(plugin, TriggerPopupList).Times(0);

        menu.DisplaySquawkAssignmentMenu(*mockFlightplan, {0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, ItDisplaysPopupListWithOptionsIfTrackedByUser)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));
        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        RECT expectedArea = {0, 0, 200, 200};
        EXPECT_CALL(plugin, TriggerPopupList(RectEq(expectedArea), "Assign Squawk", 1)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem1;
        expectedItem1.firstValue = "General";
        expectedItem1.secondValue = "";
        expectedItem1.callbackFunctionId = 55;
        expectedItem1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem1.disabled = false;
        expectedItem1.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem1)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem2;
        expectedItem2.firstValue = "Local";
        expectedItem2.secondValue = "";
        expectedItem2.callbackFunctionId = 55;
        expectedItem2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem2.disabled = false;
        expectedItem2.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem2)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem3;
        expectedItem3.firstValue = "Conspicuity (7000)";
        expectedItem3.secondValue = "";
        expectedItem3.callbackFunctionId = 55;
        expectedItem3.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem3.disabled = false;
        expectedItem3.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem3)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem4;
        expectedItem4.firstValue = "Circuit (7010)";
        expectedItem4.secondValue = "";
        expectedItem4.callbackFunctionId = 55;
        expectedItem4.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem4.disabled = false;
        expectedItem4.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem4)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem5;
        expectedItem5.firstValue = "Manual";
        expectedItem5.secondValue = "";
        expectedItem5.callbackFunctionId = 55;
        expectedItem5.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem5.disabled = false;
        expectedItem5.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem5)).Times(1);

        menu.DisplaySquawkAssignmentMenu(*mockFlightplan, {0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, ItDisplaysPopupListWithOptionsIfUntracked)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(false));
        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(false));

        RECT expectedArea = {0, 0, 200, 200};
        EXPECT_CALL(plugin, TriggerPopupList(RectEq(expectedArea), "Assign Squawk", 1)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem1;
        expectedItem1.firstValue = "General";
        expectedItem1.secondValue = "";
        expectedItem1.callbackFunctionId = 55;
        expectedItem1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem1.disabled = false;
        expectedItem1.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem1)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem2;
        expectedItem2.firstValue = "Local";
        expectedItem2.secondValue = "";
        expectedItem2.callbackFunctionId = 55;
        expectedItem2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem2.disabled = false;
        expectedItem2.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem2)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem3;
        expectedItem3.firstValue = "Conspicuity (7000)";
        expectedItem3.secondValue = "";
        expectedItem3.callbackFunctionId = 55;
        expectedItem3.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem3.disabled = false;
        expectedItem3.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem3)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem4;
        expectedItem4.firstValue = "Circuit (7010)";
        expectedItem4.secondValue = "";
        expectedItem4.callbackFunctionId = 55;
        expectedItem4.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem4.disabled = false;
        expectedItem4.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem4)).Times(1);

        UKControllerPlugin::Plugin::PopupMenuItem expectedItem5;
        expectedItem5.firstValue = "Manual";
        expectedItem5.secondValue = "";
        expectedItem5.callbackFunctionId = 55;
        expectedItem5.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expectedItem5.disabled = false;
        expectedItem5.fixedPosition = false;

        EXPECT_CALL(plugin, AddItemToPopupList(expectedItem5)).Times(1);

        menu.DisplaySquawkAssignmentMenu(*mockFlightplan, {0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesNoFlightplanOnMenuItemSelected)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, ForceGeneralSquawkForAircraft).Times(0);

        menu.MenuOptionSelected("General", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesNoRadarTargetOnMenuItemSelected)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(squawkGenerator, ForceGeneralSquawkForAircraft).Times(0);

        menu.MenuOptionSelected("General", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesGeneralSquawkMenuOption)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(
            squawkGenerator,
            ForceGeneralSquawkForAircraft(testing::Ref(*mockFlightplan), testing::Ref(*mockRadarTarget)))
            .Times(1);

        menu.MenuOptionSelected("General", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesLocalSquawkMenuOption)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(
            squawkGenerator, ForceLocalSquawkForAircraft(testing::Ref(*mockFlightplan), testing::Ref(*mockRadarTarget)))
            .Times(1);

        menu.MenuOptionSelected("Local", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesConspicuityMenuOption)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("7000", testing::Ref(*mockFlightplan))).Times(1);

        menu.MenuOptionSelected("Conspicuity (7000)", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesCircuitMenuOption)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));

        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("7010", testing::Ref(*mockFlightplan))).Times(1);

        menu.MenuOptionSelected("Circuit (7010)", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, TestItHandlesManualMenuOption)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));
        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));
        ON_CALL(*mockFlightplan, GetAssignedSquawk).WillByDefault(testing::Return("5214"));

        RECT expectedRect = {0, 0, 50, 25};
        EXPECT_CALL(plugin, ShowTextEditPopup(RectEq(expectedRect), 66, "5214")).Times(1);

        menu.MenuOptionSelected("Manual", {0, 0, 0, 0});
    }

    TEST_F(SquawkAssignmentMenuTest, ItDoesntEnterManualSquawkIfPluginReturnsNullptrFlightplan)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(nullptr));
        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("5123", testing::Ref(*mockFlightplan))).Times(0);
        menu.ManualSquawkEntered("5123");
    }

    TEST_F(SquawkAssignmentMenuTest, ItDoesntEnterManualSquawkIfPluginReturnsNullptrRadarTarget)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));
        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(nullptr));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("5123", testing::Ref(*mockFlightplan))).Times(0);
        menu.ManualSquawkEntered("5123");
    }

    TEST_F(SquawkAssignmentMenuTest, ItDoesntEnterManualSquawkIfPluginReturnsNullptrSquawkInvalid)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));
        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("XYZA", testing::Ref(*mockFlightplan))).Times(0);
        menu.ManualSquawkEntered("XYZA");
    }

    TEST_F(SquawkAssignmentMenuTest, ItEntersAManualSquawk)
    {
        ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(testing::Return(mockFlightplan));
        ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(testing::Return(mockRadarTarget));

        EXPECT_CALL(squawkGenerator, DeleteApiSquawkAndSetTo("5421", testing::Ref(*mockFlightplan))).Times(1);
        menu.ManualSquawkEntered("5421");
    }
} // namespace UKControllerPluginTest::Squawk
