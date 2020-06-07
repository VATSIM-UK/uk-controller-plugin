#include "pch/pch.h"
#include "euroscope/UserSetting.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "hold/PublishedHoldCollection.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldRendererTest : public Test
        {
            public:
                HoldRendererTest()
                    : renderer(displayManager, 1, 2),
                    displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager),
                    displayManager(new HoldDisplayManager(holdManager, displayFactory)),
                    userSetting(mockUserSettingProvider), holdManager(mockApi, mockTaskRunner),
                    dialogManager(mockDialog)
                {
                    this->holds.Add({ 1, "WILLO", "WILLO", 8000, 15000, 209, "left", {} });
                    this->holds.Add({ 2, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {} });
                    this->navaids.AddNavaid({ 1, "WILLO", EuroScopePlugIn::CPosition() });
                    this->navaids.AddNavaid({ 2, "TIMBA", EuroScopePlugIn::CPosition() });

                    displayManager->AsrLoadedEvent(this->userSetting);
                    displayManager->LoadSelectedHolds({"WILLO", "TIMBA"});
                }

                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NiceMock<MockDialogProvider> mockDialog;
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
            expected.firstValue = this->renderer.menuItemDescription;
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
            ON_CALL(this->mockUserSettingProvider, KeyExists(this->renderer.asrVisibleKey))
                .WillByDefault(Return(true));

            ON_CALL(this->mockUserSettingProvider, GetKey(this->renderer.asrVisibleKey))
                .WillByDefault(Return("0"));

            this->renderer.AsrLoadedEvent(this->userSetting);
            EXPECT_FALSE(this->renderer.IsVisible());
        }

        TEST_F(HoldRendererTest, ItDefaultsToVisisbleIfNoAsrSetting)
        {
            this->renderer.SetVisible(false);

            ON_CALL(this->mockUserSettingProvider, KeyExists(this->renderer.asrVisibleKey))
                .WillByDefault(Return(false));

            this->renderer.AsrLoadedEvent(this->userSetting);
            EXPECT_TRUE(this->renderer.IsVisible());
        }

        TEST_F(HoldRendererTest, ItMovesDisplays)
        {
            POINT expectedDisplay1 = { 212, 323 };
            POINT expectedDisplay2 = { 100, 100 };

            this->renderer.Move({ 212, 323, 600, 600 }, "1");
            EXPECT_EQ(expectedDisplay1.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay1.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay2.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay2.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidMoves)
        {
            POINT expectedDisplay = { 100, 100 };

            this->renderer.Move({ 212, 323, 600, 600 }, "5");
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesLeftClicks)
        {
            this->renderer.LeftClick(this->mockRadarScreen, 1, "TIMBA/minus", {}, {});
            EXPECT_EQ(14000, this->displayManager->GetDisplay("WILLO").GetMaximumLevel());
            EXPECT_EQ(15000, this->displayManager->GetDisplay("TIMBA").GetMaximumLevel());
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidLeftClicks)
        {
            this->renderer.LeftClick(this->mockRadarScreen, 1, "ABC/minus", {}, {});
            EXPECT_EQ(15000, this->displayManager->GetDisplay("WILLO").GetMaximumLevel());
            EXPECT_EQ(15000, this->displayManager->GetDisplay("TIMBA").GetMaximumLevel());
        }

        TEST_F(HoldRendererTest, ItResetsDisplayPositions)
        {
            POINT expectedDisplay1 = { 100, 100 };
            POINT expectedDisplay2 = { 100, 100 };

            this->renderer.Move({ 200, 200, 200, 200 }, "1");
            this->renderer.Move({ 200, 200, 200, 200 }, "2");

            this->renderer.ResetPosition();
            EXPECT_EQ(expectedDisplay1.x, this->displayManager->GetDisplay("WILLO").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay1.y, this->displayManager->GetDisplay("WILLO").GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay2.x, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay2.y, this->displayManager->GetDisplay("TIMBA").GetDisplayPos().y);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
