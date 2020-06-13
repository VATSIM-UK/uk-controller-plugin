#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/HoldSelectionMenu.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "hold/HoldingData.h"
#include "helper/Matchers.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/UserSetting.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "navaids/NavaidCollection.h"
#include "hold/PublishedHoldCollection.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldSelectionMenuTest : public Test
        {
            public:
                HoldSelectionMenuTest()
                    : displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager),
                    holdSelectionMenu(holdManager, mockPlugin, 1),
                    userSetting(mockUserSettingProvider), dialogManager(mockDialogProvider),
                    holdManager(mockApi, mockTaskRunner)
                {
                    this->mockFlightplan.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                    this->mockRadarTarget.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);
                    this->navaids.AddNavaid({ 1, "TIMBA", EuroScopePlugIn::CPosition() });
                    this->navaids.AddNavaid({ 2, "WILLO", EuroScopePlugIn::CPosition() });
                    this->navaids.AddNavaid({ 3, "MAY", EuroScopePlugIn::CPosition() });
                    this->holds.Add(std::move(this->holdData));
                    this->holds.Add(std::move(this->holdData2));
                    this->holds.Add(std::move(this->holdData3));

                    ON_CALL(*this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(this->mockUserSettingProvider, GetKey(_))
                        .WillByDefault(Return(""));
                }

                std::shared_ptr <NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr <NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NiceMock<MockDialogProvider> mockDialogProvider;
                DialogManager dialogManager;
                NavaidCollection navaids;
                PublishedHoldCollection holds;
                UserSetting userSetting;
                HoldDisplayFactory displayFactory;
                HoldManager holdManager;
                HoldSelectionMenu holdSelectionMenu;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 7000, 15000, 309, "left", {} };
                HoldingData holdData2 = { 2, "WILLO", "WILLO", 7000, 15000, 309, "left", {} };
                HoldingData holdData3 = { 3, "MAY", "MAY", 7000, 15000, 309, "left", {} };
        };

        TEST_F(HoldSelectionMenuTest, ItStartsWithNoDisplayManagers)
        {
            EXPECT_EQ(0, this->holdSelectionMenu.CountDisplayManagers());
        }

        TEST_F(HoldSelectionMenuTest, ItAddsHoldDisplayManagers)
        {
            std::shared_ptr<HoldDisplayManager> displayManager = std::make_shared<HoldDisplayManager>(
                holdManager,
                displayFactory
            );
            this->holdSelectionMenu.AddDisplayManager(displayManager);
            EXPECT_EQ(1, this->holdSelectionMenu.CountDisplayManagers());
        }

        TEST_F(HoldSelectionMenuTest, ItPopulatesTheMenuFromDisplayManagers)
        {
            std::shared_ptr<HoldDisplayManager> displayManager1 = std::make_shared<HoldDisplayManager>(
                holdManager,
                displayFactory
            );

            std::shared_ptr<HoldDisplayManager> displayManager2 = std::make_shared<HoldDisplayManager>(
                holdManager,
                displayFactory
            );

            displayManager1->AsrLoadedEvent(this->userSetting);
            displayManager2->AsrLoadedEvent(this->userSetting);
            displayManager1->LoadSelectedHolds(std::vector<std::string>({ "TIMBA", "WILLO" }));
            displayManager2->LoadSelectedHolds(std::vector<std::string>({ "WILLO", "MAY", }));
            this->holdSelectionMenu.AddDisplayManager(displayManager1);
            this->holdSelectionMenu.AddDisplayManager(displayManager2);


            PopupMenuItem menuItemHold1;
            menuItemHold1.firstValue = "TIMBA";
            menuItemHold1.secondValue = "";
            menuItemHold1.callbackFunctionId = 1;
            menuItemHold1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold1.disabled = false;
            menuItemHold1.fixedPosition = false;

            PopupMenuItem menuItemHold2;
            menuItemHold2.firstValue = "WILLO";
            menuItemHold2.secondValue = "";
            menuItemHold2.callbackFunctionId = 1;
            menuItemHold2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold2.disabled = false;
            menuItemHold2.fixedPosition = false;

            PopupMenuItem menuItemHold3;
            menuItemHold3.firstValue = "MAY";
            menuItemHold3.secondValue = "";
            menuItemHold3.callbackFunctionId = 1;
            menuItemHold3.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemHold3.disabled = false;
            menuItemHold3.fixedPosition = false;

            PopupMenuItem menuItemCancel;
            menuItemCancel.firstValue = "--";
            menuItemCancel.secondValue = "";
            menuItemCancel.callbackFunctionId = 1;
            menuItemCancel.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemCancel.disabled = false;
            menuItemCancel.fixedPosition = true;

            RECT expectedArea = { 0, 0, 300, 500 };
            EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Hold", 1))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold1))
                .Times(1);

            // WILLO is common to both managers, but should only appear once
            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold2))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemHold3))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(menuItemCancel))
                .Times(1);

            this->holdSelectionMenu.DisplayMenu(*this->mockFlightplan, *this->mockRadarTarget, "", { 0, 0 });
        }

        TEST_F(HoldSelectionMenuTest, ItDisplaysEmptyMenuIfNoHolds)
        {
            RECT expectedArea = { 0, 0, 300, 500 };
            EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Hold", 1))
                .Times(1);

            this->holdSelectionMenu.DisplayMenu(
                *this->mockFlightplan,
                *this->mockRadarTarget,
                "test",
                { 0, 0 }
            );
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemAddsAircraftToCorrespondingHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "WILLO"))
                .Times(1);

            this->holdSelectionMenu.MenuItemClicked(2, "WILLO");
            EXPECT_TRUE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("WILLO"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("TIMBA"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("MAY"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemChangesAssignedHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "WILLO"))
                .Times(1);

            this->holdManager.AssignAircraftToHold("BAW123", "TIMBA", false);
            this->holdSelectionMenu.MenuItemClicked(2, "WILLO");
            EXPECT_TRUE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("WILLO"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("TIMBA"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("MAY"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesntDoAnythingIfAlreadyAssignedToSpecifiedHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "WILLO"))
                .Times(0);

            this->holdManager.AssignAircraftToHold("BAW123", "WILLO", false);
            this->holdSelectionMenu.MenuItemClicked(2, "WILLO");
            EXPECT_TRUE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("WILLO"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("TIMBA"));
            EXPECT_FALSE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("MAY"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfFlightplanInvalid)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(std::shared_ptr<MockEuroScopeCFlightPlanInterface>(NULL)));

            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_EQ(this->holdManager.invalidAircraft, this->holdManager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfFlightplanNotTrackedByUser)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_EQ(this->holdManager.invalidAircraft, this->holdManager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, ClickingTheDashedMenuItemRemovesAircraftFromHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            this->holdManager.AssignAircraftToHold("BAW123", "TIMBA", false);
            EXPECT_TRUE(this->holdManager.GetHoldingAircraft("BAW123")->IsInHold("TIMBA"));


            EXPECT_CALL(this->mockApi, UnassignAircraftHold("BAW123"))
                .Times(1);

            this->holdSelectionMenu.MenuItemClicked(2, "--");
            EXPECT_EQ(this->holdManager.invalidAircraft, this->holdManager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, ClickingTheDashedMenuItemDoesNothingIfNotAssignedHold)
        {
            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold("BAW123"))
                .Times(0);

            this->holdSelectionMenu.MenuItemClicked(2, "--");
            EXPECT_EQ(this->holdManager.invalidAircraft, this->holdManager.GetHoldingAircraft("BAW123"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
