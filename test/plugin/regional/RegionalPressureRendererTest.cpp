#include "pch/pch.h"

#include "dialog/DialogManager.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/TestingFunctions.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "plugin/PopupMenuItem.h"
#include "regional/RegionalPressureManager.h"
#include "regional/RegionalPressureRenderer.h"
#include "regional/RegionalPressureRendererConfiguration.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using testing::Test;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Regional::RegionalPressureManager;
using UKControllerPlugin::Regional::RegionalPressureRenderer;
using UKControllerPlugin::Regional::RegionalPressureRendererConfiguration;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Regional {

        class RegionalPressureRendererTest : public Test
        {
            public:
            RegionalPressureRendererTest()
                : dialogManager(mockDialogProvider), renderer(manager, 1, 2, 3, 4, brushes, dialogManager),
                  userSettings(mockUserSettingProvider)
            {
                this->dialogManager.AddDialog(this->rpsDialogData);
            }

            DialogData rpsDialogData = {IDD_REGIONAL_PRESSURE, "Test"};
            RegionalPressureManager manager;
            GdiplusBrushes brushes;
            NiceMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            NiceMock<MockDialogProvider> mockDialogProvider;
            UserSetting userSettings;
            DialogManager dialogManager;
            RegionalPressureRenderer renderer;
        };

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultVisibilityIfNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey)).Times(1).WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSettings);
            EXPECT_TRUE(renderer.IsVisible());
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionIfNoSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSettings);
            RECT expectedArea = {100, 100, 100 + renderer.leftColumnWidth, 100 + renderer.rowHeight};
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderIfNoSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSettings);
            Gdiplus::Rect expectedArea = {100, 100, renderer.leftColumnWidth, renderer.rowHeight};
            EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionIfNoSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSettings);
            RECT expectedArea = {
                100 + renderer.leftColumnWidth,
                100,
                100 + renderer.leftColumnWidth + renderer.hideClickspotWidth,
                100 + renderer.rowHeight};
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderIfNoSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSettings);
            Gdiplus::Rect expectedArea = {
                100 + renderer.leftColumnWidth, 100, renderer.hideClickspotWidth, renderer.rowHeight};
            EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsVisibilityFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey)).Times(1).WillOnce(Return("0"));

            renderer.AsrLoadedEvent(userSettings);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultTopBarPositionFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(userSettings);
            RECT expectedArea = {150, 175, 150 + renderer.leftColumnWidth, 175 + renderer.rowHeight};
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultTopBarRenderFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(userSettings);
            Gdiplus::Rect expectedArea = {150, 175, renderer.leftColumnWidth, renderer.rowHeight};
            EXPECT_TRUE(expectedArea.Equals(renderer.GetTopBarRender()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultHideSpotPositionFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(userSettings);
            RECT expectedArea = {
                150 + renderer.leftColumnWidth,
                175,
                150 + renderer.leftColumnWidth + renderer.hideClickspotWidth,
                175 + renderer.rowHeight};
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetHideClickspotArea()));
        }

        TEST_F(RegionalPressureRendererTest, UserSettingContentLoadedSetsDefaultHideSpotRenderFromSettings)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.xPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("150"));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.yPositionUserSettingKey))
                .Times(1)
                .WillOnce(Return("175"));

            renderer.AsrLoadedEvent(userSettings);
            Gdiplus::Rect expectedArea = {
                150 + renderer.leftColumnWidth, 175, renderer.hideClickspotWidth, renderer.rowHeight};
            EXPECT_TRUE(expectedArea.Equals(renderer.GetHideSpotRender()));
        }

        TEST_F(RegionalPressureRendererTest, AsrClosingEventSavesAllValues)
        {
            // Set the values
            renderer.SetVisible(true);
            renderer.Move({100, 50, 150, 100}, "");

            RegionalPressureRendererConfiguration& config = renderer.GetConfig();
            config.AddItem({1, "bar"});
            config.AddItem({2, "baz"});
            config.AddItem({3, "foo"});

            // Expect the ASR to provider to be called appropriately.
            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.visibleUserSettingKey, renderer.visibleUserSettingDescription, "1"))
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.xPositionUserSettingKey, renderer.xPositionUserSettingDescription, "100"))
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.yPositionUserSettingKey, renderer.yPositionUserSettingDescription, "50"))
                .Times(1);

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(
                    renderer.selectedRegionalPressureUserSettingKey,
                    renderer.selecteRegionalPressureUserSettingDescription,
                    "bar;baz;foo"))
                .Times(1);

            renderer.AsrClosingEvent(userSettings);
        }

        TEST_F(RegionalPressureRendererTest, GetConfigurationMenuItemReturnsCorrectValues)
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

        TEST_F(RegionalPressureRendererTest, SelectingMenuItemOpensDialog)
        {
            renderer.SetVisible(true);

            EXPECT_CALL(mockDialogProvider, OpenDialog(this->rpsDialogData, _)).Times(1);

            renderer.Configure(0, "test", {});
        }

        TEST_F(RegionalPressureRendererTest, ClickingTheCloseClickspotMakesItInvisible)
        {
            renderer.SetVisible(true);

            EXPECT_TRUE(renderer.IsVisible());
            renderer.LeftClick(mockRadarScreen, renderer.hideClickspotId, "", {}, {});
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(RegionalPressureRendererTest, ClickingTheMinStackMarksItAsAcknowledged)
        {
            this->manager.AddRegionalPressure("ASR_LONDON", "London", 1013);

            EXPECT_FALSE(manager.GetRegionalPressure("ASR_LONDON").IsAcknowledged());
            renderer.LeftClick(mockRadarScreen, renderer.rpsClickspotId, "ASR_LONDON", {}, {});
            EXPECT_TRUE(manager.GetRegionalPressure("ASR_LONDON").IsAcknowledged());
        }

        TEST_F(RegionalPressureRendererTest, MoveShiftsTheTopBar)
        {
            Gdiplus::Rect expectedRender = {150, 50, renderer.leftColumnWidth, renderer.rowHeight};
            RECT expectedArea = {150, 50, 200, 25};

            renderer.Move(expectedArea, "");
            EXPECT_TRUE(RectsEqual(expectedArea, renderer.GetTopBarArea()));
            EXPECT_TRUE(expectedRender.Equals(renderer.GetTopBarRender()));
        }

        TEST_F(RegionalPressureRendererTest, RightClicksBehaveTheSameAsLeftClicks)
        {
            renderer.SetVisible(true);

            EXPECT_TRUE(renderer.IsVisible());
            renderer.RightClick(renderer.hideClickspotId, "", mockRadarScreen);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(RegionalPressureRendererTest, ResetPositionResetsPosition)
        {
            renderer.ResetPosition();
            EXPECT_EQ(100, renderer.GetTopBarArea().left);
            EXPECT_EQ(100, renderer.GetTopBarArea().top);
        }
    } // namespace Regional
} // namespace UKControllerPluginTest
