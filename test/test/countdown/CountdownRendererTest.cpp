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

using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Countdown::TimerConfigurationManager;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Countdown {

        class CountdownRendererTest : public Test
        {
            public:
                CountdownRendererTest()
                    : timer(mockWindows), renderer(timer, configManager, 1, 2, 3, 4, brushes),
                    configManager(dialogManager, 1), dialogManager(dialogProvider),
                    userSetting(mockUserSettingProvider)
                {
                    this->configManager.AddTimer({ 1, true, 10 });
                    this->configManager.AddTimer({ 2, false, 20 });
                    this->configManager.AddTimer({ 3, false, 30 });
                    this->configManager.AddTimer({ 4, true, 40 });
                    this->configManager.AddTimer({ 5, false, 50 });
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
        };

        TEST_F(CountdownRendererTest, AsrLoadedEventUsesDefaultsIfNoUserSettingDataExists)
        {
            // Set up the renderer
            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosX))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosY))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingVisible))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSetting);

            EXPECT_TRUE(renderer.IsVisible());
            RECT expectedTimeDisplay = { 100, 100, 100 + (2 * renderer.buttonWidth), 100 + renderer.rowHeight };
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(
                    RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.top,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom
                    },
                    renderer.GetCloseClickspotDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(1)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(2)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(3)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(4)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(5)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST_F(CountdownRendererTest, AsrLoadedEventLoadsDataFromUserSettingIfExists)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosX))
                .Times(1)
                .WillRepeatedly(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosY))
                .Times(1)
                .WillRepeatedly(Return("120"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingVisible))
                .Times(1)
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);

            EXPECT_FALSE(renderer.IsVisible());
            RECT expectedTimeDisplay = { 150, 120, 150 + (2 * renderer.buttonWidth), 120 + renderer.rowHeight };
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.top,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom
                    },
                    renderer.GetCloseClickspotDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(1)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(2)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(3)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(4)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(5)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST_F(CountdownRendererTest, MoveTransitionsTheEntireTimer)
        {
            renderer.Move({ 150, 120, 150, 120 + renderer.rowHeight }, "");

            RECT expectedTimeDisplay = { 150, 120, 150 + (2 * renderer.buttonWidth), 120 + renderer.rowHeight };
            EXPECT_TRUE(RectsEqual(expectedTimeDisplay, renderer.GetTimeDisplayArea()));
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.top,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom
                    },
                    renderer.GetCloseClickspotDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(1)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(2)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(3)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + renderer.buttonWidth,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTimerButtonArea(4)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    RECT{},
                    renderer.GetTimerButtonArea(5)
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.right,
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.right + renderer.buttonWidth,
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST_F(CountdownRendererTest, AsrClosingEventSavesAllData)
        {
            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.userSettingPosX, renderer.xPositionUserSettingDescription, "150")
                )
                .Times(1);

            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.userSettingPosY, renderer.yPositionUserSettingDescription, "120")
                )
                .Times(1);

            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.userSettingVisible, renderer.visibleUserSettingDescription, "0")
                )
                .Times(1);

            // Prepare the renderer
            renderer.SetVisible(false);
            renderer.Move({ 150, 120, 150, 120 + renderer.rowHeight }, "");
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

            EXPECT_TRUE(popup.firstValue == renderer.menuItemDescription);
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
            renderer.LeftClick(renderer.closeClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer1)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer1Toggle", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 0 && timer.GetSecondsRemaining() <= 10);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer2)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer2Toggle", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 10 && timer.GetSecondsRemaining() <= 20);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer3)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer3Toggle", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 20 && timer.GetSecondsRemaining() <= 30);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer4)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer4Toggle", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 30 && timer.GetSecondsRemaining() <= 40);
        }

        TEST_F(CountdownRendererTest, LeftClickStartsTimer5)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer5Toggle", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 40 && timer.GetSecondsRemaining() <= 50);
        }

        TEST_F(CountdownRendererTest, LeftClickHandlesUnknownTimer)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timerAToggle", mockRadarScreen);
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }

        TEST_F(CountdownRendererTest, LeftClickHandlesMissingTimerConfig)
        {
            renderer.LeftClick(renderer.functionsClickspotId, "timer6Toggle", mockRadarScreen);
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }

        TEST_F(CountdownRendererTest, LeftClickResetsTimer)
        {
            timer.StartTimer(1000);
            renderer.LeftClick(renderer.functionsClickspotId, "R", mockRadarScreen);
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
