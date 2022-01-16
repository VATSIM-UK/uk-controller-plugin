#include "aircraft/CallsignSelectionListFactory.h"
#include "euroscope/UserSetting.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "hold/PublishedHoldCollection.h"
#include "dialog/DialogManager.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldRendererTest : public Test
        {
            public:
            HoldRendererTest()
                : callsignSelectionFactory(functionHandlers, mockPlugin), dialogManager(mockDialog),
                  userSetting(mockUserSettingProvider), holdManager(mockApi, mockTaskRunner),
                  displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager, callsignSelectionFactory),
                  displayManager(new HoldDisplayManager(displayFactory)), renderer(displayManager, 1, 2)
            {
                this->holds.Add({1, "WILLO", "WILLO", 8000, 15000, 209, "left", {}});
                this->holds.Add({2, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {}});
                this->navaids.AddNavaid({1, "WILLO", EuroScopePlugIn::CPosition()});
                this->navaids.AddNavaid({2, "TIMBA", EuroScopePlugIn::CPosition()});

                displayManager->AsrLoadedEvent(this->userSetting);
                displayManager->LoadSelectedHolds({"WILLO", "TIMBA"});

                flightplan = std::make_shared<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
                ON_CALL(*flightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
                ON_CALL(*flightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

                radarTarget = std::make_shared<NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
                mockPlugin.AddAllFlightplansItem({flightplan, radarTarget});
            }

            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            FunctionCallEventHandler functionHandlers;
            CallsignSelectionListFactory callsignSelectionFactory;
            NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockTaskRunnerInterface> mockTaskRunner;
            NiceMock<MockDialogProvider> mockDialog;
            std::shared_ptr<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan;
            std::shared_ptr<NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget;
            DialogManager dialogManager;
            UserSetting userSetting;
            HoldManager holdManager;
            NavaidCollection navaids;
            PublishedHoldCollection holds;
            HoldDisplayFactory displayFactory;
            std::shared_ptr<HoldDisplayManager> displayManager;
            HoldRenderer renderer;
        };

        TEST_F(HoldRendererTest, ItReturnsAConfigurationMenuItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "Display Holds";
            expected.secondValue = "";
            expected.callbackFunctionId = 2;
            expected.checked = true;
            expected.disabled = false;
            expected.fixedPosition = false;

            EXPECT_TRUE(expected == this->renderer.GetConfigurationMenuItem());
        }

        TEST_F(HoldRendererTest, SelectingMenuItemTogglesVisibility)
        {
            EXPECT_TRUE(this->renderer.IsVisible());
            this->renderer.Configure(2, "test", {});
            EXPECT_FALSE(this->renderer.IsVisible());
            this->renderer.Configure(2, "test", {});
            EXPECT_TRUE(this->renderer.IsVisible());
        }

        TEST_F(HoldRendererTest, ItLoadsVisibilityFromTheAsr)
        {
            ON_CALL(this->mockUserSettingProvider, KeyExists("holdDisplayVisibility")).WillByDefault(Return(true));

            ON_CALL(this->mockUserSettingProvider, GetKey("holdDisplayVisibility")).WillByDefault(Return("0"));

            this->renderer.AsrLoadedEvent(this->userSetting);
            EXPECT_FALSE(this->renderer.IsVisible());
        }

        TEST_F(HoldRendererTest, ItDefaultsToVisisbleIfNoAsrSetting)
        {
            this->renderer.SetVisible(false);

            ON_CALL(this->mockUserSettingProvider, KeyExists("holdDisplayVisibility")).WillByDefault(Return(false));

            this->renderer.AsrLoadedEvent(this->userSetting);
            EXPECT_TRUE(this->renderer.IsVisible());
        }

        TEST_F(HoldRendererTest, ItMovesDisplays)
        {
            POINT expectedDisplay1 = {212, 323};
            POINT expectedDisplay2 = {100, 100};

            this->renderer.Move({212, 323, 600, 600}, "WILLO");
            EXPECT_EQ(expectedDisplay1.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay1.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay2.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay2.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidMoves)
        {
            POINT expectedDisplay = {100, 100};

            this->renderer.Move({212, 323, 600, 600}, "MAY");
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesLeftClicks)
        {
            this->renderer.LeftClick(this->mockRadarScreen, 1, "TIMBA/minus", {}, {});
            EXPECT_EQ(14000, this->displayManager->GetDisplay("TIMBA").GetMaximumLevel());
            EXPECT_EQ(15000, this->displayManager->GetDisplay("WILLO").GetMaximumLevel());
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidLeftClicks)
        {
            this->renderer.LeftClick(this->mockRadarScreen, 1, "ABC/minus", {}, {});
            EXPECT_EQ(15000, this->displayManager->GetDisplay("WILLO").GetMaximumLevel());
            EXPECT_EQ(15000, this->displayManager->GetDisplay("TIMBA").GetMaximumLevel());
        }

        TEST_F(HoldRendererTest, ItResetsDisplayPositions)
        {
            POINT expectedDisplay1 = {100, 100};
            POINT expectedDisplay2 = {100, 100};

            this->renderer.Move({200, 200, 200, 200}, "1");
            this->renderer.Move({200, 200, 200, 200}, "2");

            this->renderer.ResetPosition();
            EXPECT_EQ(expectedDisplay1.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay1.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay2.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay2.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesClearedLevelClicks)
        {
            EXPECT_CALL(
                this->mockRadarScreen,
                ToggleTemporaryAltitudePopupList("BAW123", PointEq(POINT({1, 2})), RectEq(RECT({1, 2, 3, 4}))))
                .Times(1);
            this->renderer.LeftClick(this->mockRadarScreen, 1, "TIMBA/cleared/BAW123", {1, 2}, {1, 2, 3, 4});
        }

        TEST_F(HoldRendererTest, ItHandlesCallsignClicks)
        {
            EXPECT_CALL(
                this->mockRadarScreen,
                TogglePluginTagFunction("BAW123", 9003, PointEq(POINT({1, 2})), RectEq(RECT({1, 2, 3, 4}))))
                .Times(1);
            this->renderer.LeftClick(this->mockRadarScreen, 1, "TIMBA/callsign/BAW123", {1, 2}, {1, 2, 3, 4});
        }

        TEST_F(HoldRendererTest, ItTriggersRightClicksOnHoldDisplays)
        {
            EXPECT_CALL(mockPlugin, TriggerPopupList).Times(1);

            this->renderer.RightClick(1, "TIMBA/add", this->mockRadarScreen);
        }

        TEST_F(HoldRendererTest, ItHandlesUnknownNavaidOnRightClick)
        {
            EXPECT_CALL(mockPlugin, TriggerPopupList).Times(0);

            this->renderer.RightClick(1, "BOOM/add", this->mockRadarScreen);
        }

        TEST_F(HoldRendererTest, ItHandlesUnknownButtonOnRightClick)
        {
            EXPECT_CALL(mockPlugin, TriggerPopupList).Times(0);

            this->renderer.RightClick(1, "TIMBA/boom", this->mockRadarScreen);
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
