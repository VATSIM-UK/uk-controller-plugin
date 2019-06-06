#include "pch/pch.h"
#include "minstack/MinStackRenderer.h"
#include "minstack/MinStackManager.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/TestingFunctions.h"
#include "plugin/PopupMenuItem.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "minstack/TerminalControlArea.h"

using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPlugin::MinStack::TerminalControlArea;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace MinStack {

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultVisibilityIfNoSetting)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;

            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            EXPECT_TRUE(renderer.IsVisible());
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultTopBarPositionIfNoSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            RECT expectedArea = { 100, 100, 100 + renderer.leftColumnWidth, 100 + renderer.rowHeight };
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultTopBarRenderIfNoSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            Gdiplus::Rect expectedArea = { 100, 100, renderer.leftColumnWidth, renderer.rowHeight };
            EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultHideSpotPositionIfNoSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            RECT expectedArea = {
                100 + renderer.leftColumnWidth,
                100,
                100 + renderer.leftColumnWidth + renderer.hideClickspotWidth,
                100 + renderer.rowHeight
            };
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultHideSpotRenderIfNoSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            Gdiplus::Rect expectedArea = {
                100 + renderer.leftColumnWidth,
                100, renderer.hideClickspotWidth,
                renderer.rowHeight
            };
            EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsVisibilityFromSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;

            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .Times(1)
                .WillOnce(Return("0"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultTopBarPositionFromSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            RECT expectedArea = { 150, 175, 150 + renderer.leftColumnWidth, 175 + renderer.rowHeight };
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultTopBarRenderFromSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            Gdiplus::Rect expectedArea = { 150, 175, renderer.leftColumnWidth, renderer.rowHeight };
            EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultHideSpotPositionFromSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            RECT expectedArea = {
                150 + renderer.leftColumnWidth,
                175,
                150 + renderer.leftColumnWidth + renderer.hideClickspotWidth,
                175 + renderer.rowHeight
            };
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
        }

        TEST(MinStackRenderer, UserSettingContentLoadedSetsDefaultHideSpotRenderFromSettings)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            Gdiplus::Rect expectedArea = {
                150 + renderer.leftColumnWidth,
                175,
                renderer.hideClickspotWidth,
                renderer.rowHeight
            };
            EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
        }

        TEST(MinStackRenderer, AsrClosingEventSavesAllValues)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockUserSettingProviderInterface> mockUserSettingProvider;

            // Set the values
            renderer.SetVisible(true);
            renderer.Move({ 100, 50, 150, 100 }, "");


            // Expect the ASR to provider to be called appropriately.
            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.visibleUserSettingKey, renderer.visibleUserSettingDescription, "1")
                )
                .Times(1);

            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.xPositionUserSettingKey, renderer.xPositionUserSettingDescription, "100")
                )
                .Times(1);

            EXPECT_CALL(
                    mockUserSettingProvider,
                    SetKey(renderer.yPositionUserSettingKey, renderer.yPositionUserSettingDescription, "50")
                )
                .Times(1);

            renderer.AsrClosingEvent(UserSetting(mockUserSettingProvider));
        }

        TEST(MinStackRenderer, GetConfigurationMenuItemReturnsCorrectValues)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);

            // Set the values
            renderer.SetVisible(true);
            PopupMenuItem item = renderer.GetConfigurationMenuItem();

            EXPECT_EQ(4, item.callbackFunctionId);
            EXPECT_TRUE(item.checked);
            EXPECT_FALSE(item.disabled);
            EXPECT_TRUE(renderer.menuItemDescription == item.firstValue);
            EXPECT_TRUE("" == item.secondValue);
            EXPECT_FALSE(item.fixedPosition);
        }

        TEST(MinStackRenderer, SelectingMenuItemTogglesVisibility)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);

            renderer.SetVisible(true);
            EXPECT_TRUE(renderer.IsVisible());
            renderer.Configure(0, "test", {});
            EXPECT_FALSE(renderer.IsVisible());
            renderer.Configure(0, "test", {});
            EXPECT_TRUE(renderer.IsVisible());
        }

        TEST(MinStackRenderer, ClickingTheCloseClickspotMakesItInvisible)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            renderer.SetVisible(true);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            EXPECT_TRUE(renderer.IsVisible());
            renderer.LeftClick(renderer.hideClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(MinStackRenderer, ClickingTheMinStackMarksItAsAcknowledged)
        {
            std::shared_ptr<TerminalControlArea> tma1 = std::make_shared<TerminalControlArea>(
                L"LTMA",
                "LTMA",
                6000,
                "EGLL",
                true
            );
            std::shared_ptr<TerminalControlArea> tma2 = std::make_shared<TerminalControlArea>(
                L"STMA",
                "STMA",
                6000,
                "EGPF",
                false
            );
            MinStackManager manager;
            manager.AddTerminalControlArea(tma1);
            manager.AddTerminalControlArea(tma2);

            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            EXPECT_FALSE(tma1->MinStackAcknowledged());
            renderer.LeftClick(renderer.mslClickspotId, "LTMA", mockRadarScreen);
            EXPECT_TRUE(tma1->MinStackAcknowledged());
        }

        TEST(MinStackRenderer, MoveShiftsTheTopBar)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);

            Gdiplus::Rect expectedRender = { 150, 50, renderer.leftColumnWidth, renderer.rowHeight };
            RECT expectedArea = { 150, 50, 200, 25 };

            renderer.Move(expectedArea, "");
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
            EXPECT_TRUE(expectedRender.Equals(renderer.GetTopBarRender()));
        }

        TEST(MinStackRenderer, RightClicksBehaveTheSameAsLeftClicks)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            renderer.SetVisible(true);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

            EXPECT_TRUE(renderer.IsVisible());
            renderer.RightClick(renderer.hideClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST(MinStackRenderer, ResetPositionResetsPosition)
        {
            MinStackManager manager;
            GdiplusBrushes brushes;
            MinStackRenderer renderer(manager, 1, 2, 3, 4, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;


            renderer.ResetPosition();
            EXPECT_EQ(100, renderer.GetTopBarArea().left);
            EXPECT_EQ(100, renderer.GetTopBarArea().top);
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
