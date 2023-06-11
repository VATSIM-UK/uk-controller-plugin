#include "bootstrap/PersistenceContainer.h"
#include "countdown/CountdownModule.h"
#include "countdown/CountdownTimer.h"
#include "countdown/TimerConfigurationManager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "countdown/GlobalCountdownSettingFunctions.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::Countdown::GetTimerDurationDescription;
using UKControllerPlugin::Countdown::GetTimerDurationKey;
using UKControllerPlugin::Countdown::GetTimerEnabledDescription;
using UKControllerPlugin::Countdown::GetTimerEnabledKey;
using UKControllerPlugin::Countdown::TimerConfigurationManager;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Countdown {

        class CountdownModuleTest : public Test
        {
            public:
            CountdownModuleTest()
                : countdown(windows), dialogManager(dialogProvider), userSetting(userSettingsProvider),
                  configManager(new TimerConfigurationManager(dialogManager, 1))

            {
                container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                container.dialogManager = std::make_unique<DialogManager>(dialogProvider);
                container.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
            }

            NiceMock<MockDialogProvider> dialogProvider;
            NiceMock<MockWinApi> windows;
            NiceMock<MockUserSettingProviderInterface> userSettingsProvider;
            PersistenceContainer container;
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection asrEvents;
            FunctionCallEventHandler functions;
            CountdownTimer countdown;
            DialogManager dialogManager;
            UserSetting userSetting;
            const std::shared_ptr<TimerConfigurationManager> configManager;
        };

        TEST_F(CountdownModuleTest, BootstrapPluginCreatesTimer)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(0, this->container.countdownTimer->GetSecondsRemaining());
        }

        TEST_F(CountdownModuleTest, BootstrapPluginCreatesTimerConfigurationManager)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_NO_THROW(this->container.timerConfigurationManager->CountTimers());
        }

        TEST_F(CountdownModuleTest, BootstrapPluginAddsTimerConfigsToTimer)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(5, this->container.timerConfigurationManager->CountTimers());
        }

        TEST_F(CountdownModuleTest, BootstrapPluginAddsCallbackFunctionForConfiguration)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Countdown Timer Configure"));
        }

        TEST_F(CountdownModuleTest, BootstrapPluginAddsDialog)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.userSettingHandlers->Count());
        }

        TEST_F(CountdownModuleTest, BootstrapPluginRegistersForUserSettingsEvents)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_TIMER_CONFIGURATION));
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToRendererCollection)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenSetsRenderPhaseToBeforeTags)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(1, renderables.CountRenderersInPhase(renderables.beforeTags));
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenRegistersScreenObject)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(3, renderables.CountScreenObjects());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToFunctionCallCollection)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(1, functions.CountCallbacks());
            EXPECT_EQ(0, functions.CountTagFunctions());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(2, configurables.CountDisplays());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToasrEventsEvents)
        {
            CountdownModule::BootstrapRadarScreen(
                functions, countdown, configManager, renderables, configurables, brushes, asrEvents);
            EXPECT_EQ(1, asrEvents.CountHandlers());
        }

        TEST_F(CountdownModuleTest, LoadDefaultUserSettingsBootstrapsTimerIfNotInSettings)
        {
            EXPECT_CALL(this->userSettingsProvider, KeyExists(_)).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(1))).WillRepeatedly(Return(false));

            EXPECT_CALL(this->userSettingsProvider, SetKey(GetTimerEnabledKey(1), GetTimerEnabledDescription(1), "1"))
                .Times(1);

            EXPECT_CALL(
                this->userSettingsProvider, SetKey(GetTimerDurationKey(1), GetTimerDurationDescription(1), "30"))
                .Times(1);

            CountdownModule::LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(CountdownModuleTest, LoadDefaultUserSettingsBootstrapsIgnoresTimerIfExists)
        {
            EXPECT_CALL(this->userSettingsProvider, KeyExists(_)).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(1))).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, SetKey(GetTimerEnabledKey(1), GetTimerEnabledDescription(1), "1"))
                .Times(0);

            EXPECT_CALL(
                this->userSettingsProvider, SetKey(GetTimerDurationKey(1), GetTimerDurationDescription(1), "30"))
                .Times(0);

            CountdownModule::LoadDefaultUserSettings(this->userSetting);
        }

        TEST_F(CountdownModuleTest, LoadDefaultUserSettingsBootstrapsAllTimers)
        {
            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(1))).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(2))).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(3))).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(4))).WillRepeatedly(Return(true));

            EXPECT_CALL(this->userSettingsProvider, KeyExists(GetTimerEnabledKey(5))).WillRepeatedly(Return(true));

            CountdownModule::LoadDefaultUserSettings(this->userSetting);
        }
    } // namespace Countdown
} // namespace UKControllerPluginTest
