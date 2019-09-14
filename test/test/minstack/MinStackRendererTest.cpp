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
#include "minstack/MinStackRendererConfiguration.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"

using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPlugin::MinStack::TerminalControlArea;
using UKControllerPlugin::MinStack::MinStackRendererConfiguration;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using testing::Test;

namespace UKControllerPluginTest {
    namespace MinStack {

        class MinStackRendererTest : public Test
        {
            public:
                MinStackRendererTest()
                    : dialogManager(mockDialogProvider), renderer(manager, 1, 2, 3, 4, brushes, dialogManager)
                {
                    this->dialogManager.AddDialog(this->minStackDialogData);
                }

                DialogData minStackDialogData = { IDD_MINSTACK, "Test" };
                MinStackManager manager;
                GdiplusBrushes brushes;
                NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
                NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
                NiceMock<MockDialogProvider> mockDialogProvider;
                DialogManager dialogManager;
                MinStackRenderer renderer;
        };

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultVisibilityIfNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            EXPECT_TRUE(renderer.IsVisible());
        }

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionIfNoSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderIfNoSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionIfNoSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderIfNoSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsVisibilityFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .Times(1)
                .WillOnce(Return("0"));

            renderer.AsrLoadedEvent(UserSetting(mockUserSettingProvider));
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionFromSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderFromSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionFromSettings)
        {
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

        TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderFromSettings)
        {
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

        TEST_F(MinStackRendererTest, AsrClosingEventSavesAllValues)
        {
            // Set the values
            renderer.SetVisible(true);
            renderer.Move({ 100, 50, 150, 100 }, "");

            MinStackRendererConfiguration & config = renderer.GetConfig();
            config.AddItem({ 1, "bar" });
            config.AddItem({ 2, "baz" });
            config.AddItem({ 3, "foo" });

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

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(
                    renderer.selectedMinStackUserSettingKey,
                    renderer.selectedMinStackUserSettingDescription,
                    "bar;baz;foo"
                )
            )
                .Times(1);

            renderer.AsrClosingEvent(UserSetting(mockUserSettingProvider));
        }

        TEST_F(MinStackRendererTest, GetConfigurationMenuItemReturnsCorrectValues)
        {
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

        TEST_F(MinStackRendererTest, SelectingMenuItemOpensDialog)
        {
            renderer.SetVisible(true);

            EXPECT_CALL(mockDialogProvider, OpenDialog(this->minStackDialogData, _))
                .Times(1);

            renderer.Configure(0, "test", {});
        }

        TEST_F(MinStackRendererTest, ClickingTheCloseClickspotMakesItInvisible)
        {
            renderer.SetVisible(true);

            EXPECT_TRUE(renderer.IsVisible());
            renderer.LeftClick(renderer.hideClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(MinStackRendererTest, ClickingTheMinStackMarksItAsAcknowledged)
        {
            manager.AddMsl("tma.LTMA", "tma", "LTMA", 7000);

            EXPECT_FALSE(manager.GetMinStackLevel("tma.LTMA").IsAcknowledged());
            renderer.LeftClick(renderer.mslClickspotId, "tma.LTMA", mockRadarScreen);
            EXPECT_TRUE(manager.GetMinStackLevel("tma.LTMA").IsAcknowledged());
        }

        TEST_F(MinStackRendererTest, MoveShiftsTheTopBar)
        {
            Gdiplus::Rect expectedRender = { 150, 50, renderer.leftColumnWidth, renderer.rowHeight };
            RECT expectedArea = { 150, 50, 200, 25 };

            renderer.Move(expectedArea, "");
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
            EXPECT_TRUE(expectedRender.Equals(renderer.GetTopBarRender()));
        }

        TEST_F(MinStackRendererTest, RightClicksBehaveTheSameAsLeftClicks)
        {
            renderer.SetVisible(true);

            EXPECT_TRUE(renderer.IsVisible());
            renderer.RightClick(renderer.hideClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(MinStackRendererTest, ResetPositionResetsPosition)
        {
            renderer.ResetPosition();
            EXPECT_EQ(100, renderer.GetTopBarArea().left);
            EXPECT_EQ(100, renderer.GetTopBarArea().top);
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
