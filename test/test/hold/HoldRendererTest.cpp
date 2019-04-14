#include "pch/pch.h"
#include "euroscope/UserSetting.h"
#include "hold/HoldRenderer.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/ManagedHold.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Hold::HoldRenderer;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldRendererTest : public Test
        {
            public:
                HoldRendererTest()
                    : renderer(displayManager, 1, 2), profileManager(mockApi), displayFactory(mockPlugin, holdManager),
                    displayManager(new HoldDisplayManager(profileManager, holdManager, displayFactory)),
                    userSetting(mockUserSettingProvider)
                {
                    this->holdManager.AddHold(ManagedHold({ 1, "WILLO", "WILLO", 8000, 15000, 209, "left", {} }));
                    this->holdManager.AddHold(ManagedHold({ 2, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {} }));
                    this->profileManager.AddProfile({ 1, "Test Profile", {1, 2} });
                    displayManager->AsrLoadedEvent(this->userSetting);
                    displayManager->LoadProfile(1);
                }

                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
                NiceMock<MockApiInterface> mockApi;
                UserSetting userSetting;
                HoldManager holdManager;
                HoldProfileManager profileManager;
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
            EXPECT_EQ(expectedDisplay1.x, this->displayManager->GetDisplay(1).GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay1.y, this->displayManager->GetDisplay(1).GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay2.x, this->displayManager->GetDisplay(2).GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay2.y, this->displayManager->GetDisplay(2).GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidMoves)
        {
            POINT expectedDisplay = { 100, 100 };

            this->renderer.Move({ 212, 323, 600, 600 }, "5");
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay(1).GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay(1).GetDisplayPos().y);
            EXPECT_EQ(expectedDisplay.x, this->displayManager->GetDisplay(2).GetDisplayPos().x);
            EXPECT_EQ(expectedDisplay.y, this->displayManager->GetDisplay(2).GetDisplayPos().y);
        }

        TEST_F(HoldRendererTest, ItHandlesLeftClicks)
        {
            this->renderer.LeftClick(1, "1/minus", this->mockRadarScreen);
            EXPECT_EQ(1, this->displayManager->GetDisplay(1).GetLevelsSkipped());
            EXPECT_EQ(0, this->displayManager->GetDisplay(2).GetLevelsSkipped());
        }

        TEST_F(HoldRendererTest, ItHandlesInvalidLeftClicks)
        {
            this->renderer.LeftClick(1, "55/minus", this->mockRadarScreen);


            EXPECT_EQ(0, this->displayManager->GetDisplay(1).GetLevelsSkipped());
            EXPECT_EQ(0, this->displayManager->GetDisplay(2).GetLevelsSkipped());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
