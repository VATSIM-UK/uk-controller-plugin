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

using UKControllerPlugin::Countdown::CountdownRenderer;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::Return;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Countdown {

        TEST(CountdownRenderer, AsrLoadedEventUsesDefaultsIfNoUserSettingDataExists)
        {
            // Set up the renderer
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            // Mock the ASR provider.
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosX))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosY))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingVisible))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));

            EXPECT_TRUE(renderer.IsVisible());
            RECT expectedTimeDisplay = { 100, 100, 100 + renderer.timeDisplayWidth, 100 + renderer.rowHeight };
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
                    renderer.GetThirtySecondDisplayArea()
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
                    renderer.GetOneMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetNinetySecondDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTwoMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetThreeMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (6 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST(CountdownRenderer, UserSettingContentLoadedEventLoadsDataFromUserSettingIfExists)
        {
            // Set up the renderer
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            // Mock the ASR provider.
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosX))
                .Times(1)
                .WillRepeatedly(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingPosY))
                .Times(1)
                .WillRepeatedly(Return("120"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.userSettingVisible))
                .Times(1)
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));

            EXPECT_FALSE(renderer.IsVisible());
            RECT expectedTimeDisplay = { 150, 120, 150 + renderer.timeDisplayWidth, 120 + renderer.rowHeight };
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
                    renderer.GetThirtySecondDisplayArea()
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
                    renderer.GetOneMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetNinetySecondDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTwoMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetThreeMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (6 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST(CountdownRenderer, MoveTransitionsTheEntireTimer)
        {
            // Set up the renderer
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            renderer.Move({ 150, 120, 150 + renderer.timeDisplayWidth, 120 + renderer.rowHeight });

            RECT expectedTimeDisplay = { 150, 120, 150 + renderer.timeDisplayWidth, 120 + renderer.rowHeight };
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
                    renderer.GetThirtySecondDisplayArea()
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
                    renderer.GetOneMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (2 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetNinetySecondDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (3 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetTwoMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (4 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetThreeMinuteDisplayArea()
                )
            );
            EXPECT_TRUE(
                RectsEqual(
                    {
                        expectedTimeDisplay.left + (5 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom,
                        expectedTimeDisplay.left + (6 * renderer.buttonWidth),
                        expectedTimeDisplay.bottom + renderer.rowHeight
                    },
                    renderer.GetResetDisplayArea()
                )
            );
        }

        TEST(CountdownRenderer, AsrClosingEventSavesAllData)
        {
            // Set up the renderer
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            // Mock the ASR provider.
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
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
            renderer.Move({ 150, 120, 150 + renderer.timeDisplayWidth, 120 + renderer.rowHeight });
            renderer.AsrClosingEvent(UserSetting(mockUserSettingProvider));
        }

        TEST(CountdownRenderer, ConfigureToggleVisibility)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            renderer.SetVisible(false);
            EXPECT_FALSE(renderer.IsVisible());
            renderer.Configure(0, "test");
            EXPECT_TRUE(renderer.IsVisible());
            renderer.Configure(0, "test");
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(CountdownRenderer, GetConfigurationMenuItemReturnsCorrectData)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            renderer.SetVisible(true);
            PopupMenuItem popup = renderer.GetConfigurationMenuItem();

            EXPECT_TRUE(popup.firstValue == renderer.menuItemDescription);
            EXPECT_TRUE(popup.secondValue == "");
            EXPECT_TRUE(popup.callbackFunctionId == 4);
            EXPECT_TRUE(popup.checked);
            EXPECT_FALSE(popup.disabled);
            EXPECT_FALSE(popup.fixedPosition);
        }

        TEST(CountdownRenderer, IsVisibleReturnsVisibility)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            renderer.SetVisible(true);
            EXPECT_TRUE(renderer.IsVisible());
            renderer.SetVisible(false);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(CountdownRenderer, LeftClickCloseClickspotSetsInvisible)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;
            renderer.SetVisible(true);
            renderer.LeftClick(renderer.closeClickspotId, "", mockRadar);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(CountdownRenderer, LeftClickStartsThirtySecondTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            renderer.LeftClick(renderer.functionsClickspotId, "0.5", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 0 && timer.GetSecondsRemaining() <= 30);
        }

        TEST(CountdownRenderer, LeftClickStartsOneMinuteTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            renderer.LeftClick(renderer.functionsClickspotId, "1", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 0 && timer.GetSecondsRemaining() <= 60);
        }

        TEST(CountdownRenderer, LeftClickStartsNinetySecondTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            renderer.LeftClick(renderer.functionsClickspotId, "1.5", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 0 && timer.GetSecondsRemaining() <= 90);
        }

        TEST(CountdownRenderer, LeftClickStartsTwoMinuteTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            renderer.LeftClick(renderer.functionsClickspotId, "2", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 60 && timer.GetSecondsRemaining() <= 120);
        }

        TEST(CountdownRenderer, LeftClickStartsThreeMinuteTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            renderer.LeftClick(renderer.functionsClickspotId, "3", mockRadarScreen);
            EXPECT_TRUE(timer.GetSecondsRemaining() > 120 && timer.GetSecondsRemaining() <= 180);
        }

        TEST(CountdownRenderer, LeftClickResetsTimer)
        {
            StrictMock<MockWinApi> mockWindows;
            CountdownTimer timer(mockWindows);
            GdiplusBrushes brushes;
            CountdownRenderer renderer(timer, 1, 2, 3, 4, brushes);

            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            timer.StartTimer(1000);
            renderer.LeftClick(renderer.functionsClickspotId, "R", mockRadarScreen);
            EXPECT_EQ(0, timer.GetSecondsRemaining());
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
