#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/TestingFunctions.h"
#include "minstack/MinStackLevel.h"
#include "minstack/MinStackManager.h"
#include "minstack/MinStackRenderer.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using testing::Test;

using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackRenderer;
using UKControllerPlugin::MinStack::MinStackRendererConfiguration;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::MinStack {

    class MinStackRendererTest : public Test
    {
        public:
        MinStackRendererTest()
            : dialogManager(mockDialogProvider), settings(mockUserSettingProvider),
              renderer(manager, 1, 2, 3, 4, brushes, dialogManager)

        {
            this->dialogManager.AddDialog(this->minStackDialogData);
        }

        DialogData minStackDialogData = {IDD_MINSTACK, "Test"};
        MinStackManager manager;
        GdiplusBrushes brushes;
        NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        NiceMock<MockDialogProvider> mockDialogProvider;
        DialogManager dialogManager;
        UserSetting settings;
        MinStackRenderer renderer;
    };

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultVisibilityIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.VisibleUserSettingKey())).Times(1).WillOnce(Return(""));

        renderer.AsrLoadedEvent(settings);
        EXPECT_TRUE(renderer.IsVisible());
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionIfNoSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        renderer.AsrLoadedEvent(settings);
        RECT expectedArea = {100, 100, 100 + renderer.LeftColumnWidth(), 100 + renderer.RowHeight()};
        EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderIfNoSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        renderer.AsrLoadedEvent(settings);
        Gdiplus::Rect expectedArea = {100, 100, renderer.LeftColumnWidth(), renderer.RowHeight()};
        EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionIfNoSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        renderer.AsrLoadedEvent(settings);
        RECT expectedArea = {
            100 + renderer.LeftColumnWidth(),
            100,
            100 + renderer.LeftColumnWidth() + renderer.HideClickspotWidth(),
            100 + renderer.RowHeight()};
        EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderIfNoSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey())).Times(1).WillOnce(Return(""));

        renderer.AsrLoadedEvent(settings);
        Gdiplus::Rect expectedArea = {
            100 + renderer.LeftColumnWidth(), 100, renderer.HideClickspotWidth(), renderer.RowHeight()};
        EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsVisibilityFromSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.VisibleUserSettingKey())).Times(1).WillOnce(Return("0"));

        renderer.AsrLoadedEvent(settings);
        EXPECT_FALSE(renderer.IsVisible());
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionFromSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("150"));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("175"));

        renderer.AsrLoadedEvent(settings);
        RECT expectedArea = {150, 175, 150 + renderer.LeftColumnWidth(), 175 + renderer.RowHeight()};
        EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderFromSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("150"));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("175"));

        renderer.AsrLoadedEvent(settings);
        Gdiplus::Rect expectedArea = {150, 175, renderer.LeftColumnWidth(), renderer.RowHeight()};
        EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionFromSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("150"));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("175"));

        renderer.AsrLoadedEvent(settings);
        RECT expectedArea = {
            150 + renderer.LeftColumnWidth(),
            175,
            150 + renderer.LeftColumnWidth() + renderer.HideClickspotWidth(),
            175 + renderer.RowHeight()};
        EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
    }

    TEST_F(MinStackRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderFromSettings)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.XPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("150"));

        EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.YPositionUserSettingKey()))
            .Times(1)
            .WillOnce(Return("175"));

        renderer.AsrLoadedEvent(settings);
        Gdiplus::Rect expectedArea = {
            150 + renderer.LeftColumnWidth(), 175, renderer.HideClickspotWidth(), renderer.RowHeight()};
        EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
    }

    TEST_F(MinStackRendererTest, AsrClosingEventSavesAllValues)
    {
        // Set the values
        renderer.SetVisible(true);
        renderer.Move({100, 50, 150, 100}, "");

        MinStackRendererConfiguration& config = renderer.GetConfig();
        config.AddItem({1, "bar"});
        config.AddItem({2, "baz"});
        config.AddItem({3, "foo"});

        // Expect the ASR to provider to be called appropriately.
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey(renderer.VisibleUserSettingKey(), renderer.VisibleUserSettingDescription(), "1"))
            .Times(1);

        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey(renderer.XPositionUserSettingKey(), renderer.XPositionUserSettingDescription(), "100"))
            .Times(1);

        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey(renderer.YPositionUserSettingKey(), renderer.YPositionUserSettingDescription(), "50"))
            .Times(1);

        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey(
                renderer.SelectedMinStackUserSettingKey(),
                renderer.SelectedMinStackUserSettingDescription(),
                "bar;baz;foo"))
            .Times(1);

        renderer.AsrClosingEvent(settings);
    }

    TEST_F(MinStackRendererTest, GetConfigurationMenuItemReturnsCorrectValues)
    {
        // Set the values
        renderer.SetVisible(true);
        PopupMenuItem item = renderer.GetConfigurationMenuItem();

        EXPECT_EQ(4, item.callbackFunctionId);
        EXPECT_TRUE(item.checked);
        EXPECT_FALSE(item.disabled);
        EXPECT_TRUE(renderer.MenuItemDescription() == item.firstValue);
        EXPECT_TRUE(item.secondValue.empty());
        EXPECT_FALSE(item.fixedPosition);
    }

    TEST_F(MinStackRendererTest, SelectingMenuItemOpensDialog)
    {
        renderer.SetVisible(true);

        EXPECT_CALL(mockDialogProvider, OpenDialog(this->minStackDialogData, _)).Times(1);

        renderer.Configure(0, "test", {});
    }

    TEST_F(MinStackRendererTest, ClickingTheCloseClickspotMakesItInvisible)
    {
        renderer.SetVisible(true);

        EXPECT_TRUE(renderer.IsVisible());
        renderer.LeftClick(mockRadarScreen, renderer.HideClickspotId(), "", {}, {});
        EXPECT_FALSE(renderer.IsVisible());
    }

    TEST_F(MinStackRendererTest, ClickingTheMinStackMarksItAsAcknowledged)
    {
        manager.AddMsl("tma.LTMA", "tma", "LTMA", 7000);

        EXPECT_FALSE(manager.GetMinStackLevel("tma.LTMA").IsAcknowledged());
        renderer.LeftClick(mockRadarScreen, renderer.MslClickspotId(), "tma.LTMA", {}, {});
        EXPECT_TRUE(manager.GetMinStackLevel("tma.LTMA").IsAcknowledged());
    }

    TEST_F(MinStackRendererTest, MoveShiftsTheTopBar)
    {
        Gdiplus::Rect expectedRender = {150, 50, renderer.LeftColumnWidth(), renderer.RowHeight()};
        RECT expectedArea = {150, 50, 200, 25};

        renderer.Move(expectedArea, "");
        EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
        EXPECT_TRUE(expectedRender.Equals(renderer.GetTopBarRender()));
    }

    TEST_F(MinStackRendererTest, RightClicksBehaveTheSameAsLeftClicks)
    {
        renderer.SetVisible(true);

        EXPECT_TRUE(renderer.IsVisible());
        renderer.RightClick(renderer.HideClickspotId(), "", mockRadarScreen);
        EXPECT_FALSE(renderer.IsVisible());
    }

    TEST_F(MinStackRendererTest, ResetPositionResetsPosition)
    {
        renderer.ResetPosition();
        EXPECT_EQ(100, renderer.GetTopBarArea().left);
        EXPECT_EQ(100, renderer.GetTopBarArea().top);
    }
} // namespace UKControllerPluginTest::MinStack
