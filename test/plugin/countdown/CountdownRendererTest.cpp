#include "pch/pch.h"
#include "countdown/CountdownRenderer.h"
#include "graphics/GdiplusBrushes.h"
#include "countdown/CountdownTimer.h"
#include "mock/MockWinApi.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "helper/TestingFunctions.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"
#include "countdown/TimerConfigurationManager.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::Countdown::TimerConfigurationManager;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Countdown {

        class CountdownRendererTest : public Test
        {
            public:
            CountdownRendererTest()
                : userSetting(mockUserSettingProvider), dialogManager(dialogProvider), timer(mockWindows),
                  configManager(dialogManager, 1), renderer(timer, configManager, 1, 2, 3, 4, brushes)

            {
                this->configManager.AddTimer({1, true, 10});
                this->configManager.AddTimer({2, false, 20});
                this->configManager.AddTimer({3, false, 30});
                this->configManager.AddTimer({4, true, 40});
                this->configManager.AddTimer({5, false, 50});
            }

            NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            NiceMock<MockDialogProvider> dialogProvider;
            NiceMock<MockWinApi> mockWindows;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting;
            DialogManager dialogManager;
            GdiplusBrushes brushes;
            CountdownTimer timer;
            TimerConfigurationManager configManager;
            CountdownRenderer renderer;

            const std::string POS_X_ASR_ENTRY = "CountdownScreenPosX";
            const std::string POS_X_ASR_DESC = "Countdown X Position";
            const std::string POS_Y_ASR_ENTRY = "CountdownScreenPosY";
            const std::string POS_Y_ASR_DESC = "Countdown Y Position";
            const std::string VISIBLE_ASR_ENTRY = "DisplayCountdown";
            const std::string VISIBLE_ASR_DESC = "Display UKCP Countdown Timer";
            const int BUTTON_WIDTH = 35;
            const int ROW_HEIGHT = 20;
        };

        TEST_F(CountdownRendererTest, AsrLoadedEventUsesDefaultsIfNoUserSettingDataExists)
        {
            // Set up the renderer
            EXPECT_CALL(mockUserSettingProvider, GetKey(POS_X_ASR_ENTRY)).Times(1).WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(POS_X_ASR_ENTRY)).Times(1).WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(VISIBLE_ASR_ENTRY)).Times(1).WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSetting);

            EXPECT_TRUE(renderer.IsVisible());
            RECT expectedTimeDisplay = {100, 100, 100 + (2 * BUTTON_WIDTH), 100 + ROW_HEIGHT};
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.top,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom},
                renderer.GetCloseClickspotDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(1)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(2)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(3)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + (2 * BUTTON_WIDTH),
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(4)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(5)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetResetDisplayArea()));
        }

        TEST_F(CountdownRendererTest, AsrLoadedEventLoadsDataFromUserSettingIfExists)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(POS_X_ASR_ENTRY)).Times(1).WillRepeatedly(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(POS_X_ASR_ENTRY)).Times(1).WillRepeatedly(Return("120"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(VISIBLE_ASR_ENTRY)).Times(1).WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);

            EXPECT_FALSE(renderer.IsVisible());
            RECT expectedTimeDisplay = {150, 120, 150 + (2 * BUTTON_WIDTH), 120 + ROW_HEIGHT};
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.top,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom},
                renderer.GetCloseClickspotDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(1)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(2)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(3)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + (2 * BUTTON_WIDTH),
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(4)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(5)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetResetDisplayArea()));
        }

        TEST_F(CountdownRendererTest, MoveTransitionsTheEntireTimer)
        {
            renderer.Move({150, 120, 150, 120 + ROW_HEIGHT}, "");

            RECT expectedTimeDisplay = {150, 120, 150 + (2 * BUTTON_WIDTH), 120 + ROW_HEIGHT};
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.top,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom},
                renderer.GetCloseClickspotDisplayArea()));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(1)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(2)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(3)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.left + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.left + (2 * BUTTON_WIDTH),
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetTimerButtonArea(4)));
            EXPECT_TRUE(RectsEqual(RECT{}, renderer.GetTimerButtonArea(5)));
            EXPECT_TRUE(RectsEqual(
                {expectedTimeDisplay.right,
                 expectedTimeDisplay.bottom,
                 expectedTimeDisplay.right + BUTTON_WIDTH,
                 expectedTimeDisplay.bottom + ROW_HEIGHT},
                renderer.GetResetDisplayArea()));
        }

        TEST_F(CountdownRendererTest, AsrClosingEventSavesAllData)
        {
            EXPECT_CALL(mockUserSettingProvider, SetKey(POS_X_ASR_ENTRY, POS_X_ASR_DESC, "150")).Times(1);

            EXPECT_CALL(mockUserSettingProvider, SetKey(POS_X_ASR_ENTRY, POS_Y_ASR_DESC, "120")).Times(1);

            EXPECT_CALL(mockUserSettingProvider, SetKey(VISIBLE_ASR_ENTRY, VISIBLE_ASR_DESC, "0")).Times(1);

            // Prepare the renderer
            renderer.SetVisible(false);
            renderer.Move({150, 120, 150, 120 + ROW_HEIGHT}, "");
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(CountdownRendererTest, ConfigureToggleVisibility)
        {
            renderer.SetVisible(false);
            EXPECT_FALSE(renderer.IsVisible());
            renderer.Configure(0, "test", {});
            EXPECT_TRUE(renderer.IsVisible());
            renderer.Configure(0, "test", {});
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(CountdownRendererTest, GetConfigurationMenuItemReturnsCorrectData)
        {
            renderer.SetVisible(true);
            PopupMenuItem popup = renderer.GetConfigurationMenuItem();

            EXPECT_TRUE(popup.firstValue == "Display Countdown Timer");
            EXPECT_TRUE(popup.secondValue == "");
            EXPECT_TRUE(popup.callbackFunctionId == 4);
            EXPECT_TRUE(popup.checked);
            EXPECT_FALSE(popup.disabled);
            EXPECT_FALSE(popup.fixedPosition);
        }

        TEST_F(CountdownRendererTest, IsVisibleReturnsVisibility)
        {
            renderer.SetVisible(true);
            EXPECT_TRUE(renderer.IsVisible());
            renderer.SetVisible(false);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(CountdownRendererTest, LeftClickCloseClickspotSetsInvisible)
        {
            renderer.SetVisible(true);
            renderer.LeftClick(mockRadarScreen, 3, "", {}, {});
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer1)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer1Toggle", {}, {});
            EXPECT_TRUE(timer.GetSecondsRemaining() > 0 && timer.GetSecondsRemaining() <= 10);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer2)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer2Toggle", {}, {});
            EXPECT_TRUE(timer.GetSecondsRemaining() > 10 && timer.GetSecondsRemaining() <= 20);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer3)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer3Toggle", {}, {});
            EXPECT_TRUE(timer.GetSecondsRemaining() > 20 && timer.GetSecondsRemaining() <= 30);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer4)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer4Toggle", {}, {});
            EXPECT_TRUE(timer.GetSecondsRemaining() > 30 && timer.GetSecondsRemaining() <= 40);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer5)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer5Toggle", {}, {});
            EXPECT_TRUE(timer.GetSecondsRemaining() > 40 && timer.GetSecondsRemaining() <= 50);
        }

        TEST_F(CountdownRendererTest, LeftClickHandlesUnknownTimer)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timerAToggle", {}, {});
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }

        TEST_F(CountdownRendererTest, LeftClickHandlesMissingTimerConfig)
        {
            renderer.LeftClick(mockRadarScreen, 1, "timer6Toggle", {}, {});
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }

        TEST_F(CountdownRendererTest, LeftClickResetsTimer)
        {
            timer.StartTimer(1000);
            renderer.LeftClick(mockRadarScreen, 1, "R", {}, {});
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }

        TEST_F(CountdownRendererTest, ResetPositionSetsPosition)
        {
            renderer.ResetPosition();
            EXPECT_EQ(100, renderer.GetTimeDisplayArea().left);
            EXPECT_EQ(100, renderer.GetTimeDisplayArea().top);
        }
    } // namespace Countdown
} // namespace UKControllerPluginTest
