#include "pch/pch.h"
#include "historytrail/HistoryTrailRenderer.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "mock/MockWinApi.h"
#include "historytrail/HistoryTrailRepository.h"
#include "plugin/PopupMenuItem.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailRenderer;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace HistoryTrail {

        class HistoryTrailRendererTest : public Test
        {
            public:

                HistoryTrailRendererTest(void)
                    : userSetting(mockUserSettingProvider), renderer(repo, mockPlugin, mockWindows, 1)
                {

                }

            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            HistoryTrailRepository repo;
            NiceMock<MockWinApi> mockWindows;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UserSetting userSetting;
            HistoryTrailRenderer renderer;
        };

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDefaultVisibilityIfNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_TRUE(renderer.IsVisible());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDefaultTrailTypeNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.trailTypeUserSettingKey))
                .Times(1)
                .WillOnce(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(0, renderer.GetHistoryTrailType());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDefaultDotSizeNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.dotSizeUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(4, renderer.GetHistoryTrailDotSize());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDegradingTrailsNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.degradingUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_TRUE(renderer.GetDegradingTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsFadingTrailsNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.fadingUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_TRUE(renderer.GetFadingTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsAntiAliasNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.antialiasUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_TRUE(renderer.GetAntiAliasedTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsAlphaPerDotNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(renderer.GetAlphaPerDot(), 255 / renderer.GetHistoryTrailLength());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsTrailColourNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.trailColourUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            Gdiplus::Color & color = renderer.GetTrailColour();
            EXPECT_EQ(color.GetA(), 255);
            EXPECT_EQ(color.GetR(), 255);
            EXPECT_EQ(color.GetG(), 130);
            EXPECT_EQ(color.GetB(), 20);
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsMinAltitudeNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.minAltitudeFilterUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(renderer.defaultMinAltitude, renderer.GetMinimumAltitudeFilter());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsMaxAltitudeNoSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.maxAltitudeFilterUserSettingKey))
                .Times(1)
                .WillRepeatedly(Return(""));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(renderer.defaultMaxAltitude, renderer.GetMaximumAltitudeFilter());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsVisibilityFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.visibleUserSettingKey))
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_FALSE(renderer.IsVisible());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDefaultTrailTypeFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.trailTypeUserSettingKey))
                .WillRepeatedly(Return("1"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(1, renderer.GetHistoryTrailType());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDefaultDotSizeFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.dotSizeUserSettingKey))
                .WillRepeatedly(Return("50"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(50, renderer.GetHistoryTrailDotSize());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsDegradingTrailsFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.degradingUserSettingKey))
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_FALSE(renderer.GetDegradingTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsFadingTrailsFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.fadingUserSettingKey))
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_FALSE(renderer.GetFadingTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsAntiAliasFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.antialiasUserSettingKey))
                .WillRepeatedly(Return("0"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_FALSE(renderer.GetAntiAliasedTrails());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsTrailColourFromUserSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.trailColourUserSettingKey))
                .WillRepeatedly(Return("190,247,235"));

            renderer.AsrLoadedEvent(userSetting);
            Gdiplus::Color color = renderer.GetTrailColour();
            EXPECT_EQ(color.GetA(), 255);
            EXPECT_EQ(color.GetR(), 190);
            EXPECT_EQ(color.GetG(), 247);
            EXPECT_EQ(color.GetB(), 235);
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsMinAltitudeFromSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.minAltitudeFilterUserSettingKey))
                .WillRepeatedly(Return("5000"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(5000, renderer.GetMinimumAltitudeFilter());
        }

        TEST_F(HistoryTrailRendererTest, AsrLoadedEventSetsMaxAltitudeFromSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, GetKey(renderer.maxAltitudeFilterUserSettingKey))
                .WillRepeatedly(Return("15000"));

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_EQ(15000, renderer.GetMaximumAltitudeFilter());
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesVisibleSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.visibleUserSettingKey, renderer.visibleUserSettingDescription, "1")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesTypeSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.trailTypeUserSettingKey, renderer.trailTypeUserSettingDescription, "0")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesSizeSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.dotSizeUserSettingKey, renderer.dotSizeUserSettingDescription, "4")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesDegradingSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.degradingUserSettingKey, renderer.degradingUserSettingDescription, "1")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesFadingSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.fadingUserSettingKey, renderer.fadingUserSettingDescription, "1")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesLengthSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.trailLengthUserSettingKey, renderer.trailLengthUserSettingDescription, "15")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesColourSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.trailColourUserSettingKey, renderer.trailColourUserSettingDescription, "255,130,20")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesAntialiasSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(renderer.antialiasUserSettingKey, renderer.antialiasUserSettingDescription, "1")
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesMinAltitudeSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(
                    renderer.minAltitudeFilterUserSettingKey,
                    renderer.minAltitudeFilterUserSettingDescription,
                    "0"
                )
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, AsrClosingEventSavesMaxAltitudeSetting)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockUserSettingProvider, SetKey(_, _, _))
                .WillRepeatedly(Return());

            EXPECT_CALL(
                mockUserSettingProvider,
                SetKey(
                renderer.maxAltitudeFilterUserSettingKey,
                renderer.maxAltitudeFilterUserSettingDescription,
                "99999"
            )
            )
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.AsrClosingEvent(userSetting);
        }

        TEST_F(HistoryTrailRendererTest, ConfigureOpensPopup)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockWindows, OpenDialog(102, _, _))
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            renderer.Configure(0, "test");
        }

        TEST_F(HistoryTrailRendererTest, GetConfigurationMenuItemSetsRightValues)
        {
            PopupMenuItem item = renderer.GetConfigurationMenuItem();
            EXPECT_EQ(1, item.callbackFunctionId);
            EXPECT_TRUE(item.checked);
            EXPECT_FALSE(item.disabled);
            EXPECT_TRUE(renderer.menuItemDescription == item.firstValue);
            EXPECT_TRUE("" == item.secondValue);
            EXPECT_FALSE(item.fixedPosition);
        }

        TEST_F(HistoryTrailRendererTest, ProcessCommandReturnsTrueOnCorrectCommand)
        {
            EXPECT_CALL(mockUserSettingProvider, GetKey(_))
                .WillRepeatedly(Return(""));

            EXPECT_CALL(mockWindows, OpenDialog(102, _, _))
                .Times(1);

            renderer.AsrLoadedEvent(userSetting);
            EXPECT_TRUE(renderer.ProcessCommand(".ukcp h"));
        }

        TEST_F(HistoryTrailRendererTest, ProcessCommandReturnsFalseOnIncorrectCommand)
        {
            EXPECT_FALSE(renderer.ProcessCommand(".ukcp h 2"));
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
