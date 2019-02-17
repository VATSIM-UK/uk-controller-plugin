#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldSelectionMenu.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingData.h"
#include "helper/Matchers.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldSelectionMenu;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldSelectionMenuTest : public Test
        {
            public:
                HoldSelectionMenuTest()
                    : holdSelectionMenu(this->holdManager, this->mockPlugin, 1)
                {
                    this->mockFlightplan.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                    this->mockRadarTarget.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);
                    this->holdManager.AddHold(ManagedHold(this->holdData));
                    this->holdManager.AddHold(ManagedHold(this->holdData2));
                    this->holdManager.AddHold(ManagedHold(this->holdData3));

                    ON_CALL(*this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                }

                std::shared_ptr <NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr <NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager holdManager;
                HoldSelectionMenu holdSelectionMenu;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 7000, 15000, 309, 1 };
                HoldingData holdData2 = { 2, "WILLO", "WILLO", 7000, 15000, 309, 1 };
                HoldingData holdData3 = { 3, "MAY", "MAY", 7000, 15000, 309, 1 };
        };

        TEST_F(HoldSelectionMenuTest, ItStartsWithNoHolds)
        {
            EXPECT_EQ(0, this->holdSelectionMenu.CountHolds());
        }

        TEST_F(HoldSelectionMenuTest, ItAddsHolds)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            EXPECT_EQ(1, this->holdSelectionMenu.CountHolds());
        }

        TEST_F(HoldSelectionMenuTest, ItDoesntAddDuplicateHolds)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.AddHoldToMenu(1);
            EXPECT_EQ(1, this->holdSelectionMenu.CountHolds());
        }

        TEST_F(HoldSelectionMenuTest, ItRemovesHolds)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.RemoveHoldFromMenu(1);
            EXPECT_EQ(0, this->holdSelectionMenu.CountHolds());
        }

        TEST_F(HoldSelectionMenuTest, ItGracefullyHandlesRemovingNonExistantHolds)
        {
            EXPECT_NO_THROW(this->holdSelectionMenu.RemoveHoldFromMenu(1));
        }

        TEST_F(HoldSelectionMenuTest, ItDoesntDisplayTheMenuIfNoHolds)
        {
            EXPECT_CALL(this->mockPlugin, TriggerPopupList(_, _, _))
                .Times(0);

            this->holdSelectionMenu.DisplayMenu(
                *this->mockFlightplan,
                *this->mockRadarTarget,
                "test",
                { 0, 0 }
            );
        }

        TEST_F(HoldSelectionMenuTest, ItDisplaysTheMenu)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.AddHoldToMenu(2);


            RECT expectedArea = {
                0,
                0,
                300,
                500
            };

            EXPECT_CALL(this->mockPlugin, TriggerPopupList(RectEq(expectedArea), "Hold Selection For BAW123", 1))
                .Times(1);

            PopupMenuItem expectedItem1;
            expectedItem1.firstValue = "TIMBA";
            expectedItem1.secondValue = "";
            expectedItem1.callbackFunctionId = 2;
            expectedItem1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expectedItem1.disabled = false;
            expectedItem1.fixedPosition = false;

            PopupMenuItem expectedItem2;
            expectedItem2.firstValue = "WILLO";
            expectedItem2.secondValue = "";
            expectedItem2.callbackFunctionId = 3;
            expectedItem2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expectedItem2.disabled = false;
            expectedItem2.fixedPosition = false;

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(PopupMenuItemEq(expectedItem1)))
                .Times(1);

            EXPECT_CALL(this->mockPlugin, AddItemToPopupList(PopupMenuItemEq(expectedItem2)))
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
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.AddHoldToMenu(2);


            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(this->mockRadarTarget));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_TRUE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfFlightplanInvalid)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.AddHoldToMenu(2);


            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(std::shared_ptr<MockEuroScopeCFlightPlanInterface>(NULL)));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(this->mockRadarTarget));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_FALSE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }

        TEST_F(HoldSelectionMenuTest, SelectingAMenuItemDoesNothingIfRadarTargetInvalid)
        {
            this->holdSelectionMenu.AddHoldToMenu(1);
            this->holdSelectionMenu.AddHoldToMenu(2);


            ON_CALL(this->mockPlugin, GetSelectedFlightplan())
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetSelectedRadarTarget())
                .WillByDefault(Return(std::shared_ptr<MockEuroScopeCRadarTargetInterface>(NULL)));

            // Number 2 - that's hold 1 (base + id);
            this->holdSelectionMenu.MenuItemClicked(2, "test");
            EXPECT_FALSE(this->holdManager.GetManagedHold(1)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(2)->HasAircraft("BAW123"));
            EXPECT_FALSE(this->holdManager.GetManagedHold(3)->HasAircraft("BAW123"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest