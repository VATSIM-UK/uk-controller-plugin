#include "pch/pch.h"
#include "bootstrap/PersistenceContainer.h"
#include "countdown/CountdownModule.h"
#include "countdown/CountdownTimer.h"
#include "countdown/TimerConfigurationManager.h"
#include "mock/MockWinApi.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "euroscope/UserSetting.h"
#include "mock/MockUserSettingProviderInterface.h"

using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPlugin::Countdown::TimerConfigurationManager;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Countdown {

        class CountdownModuleTest : public Test
        {
            public:
                CountdownModuleTest()
                    : countdown(windows), configManager(new TimerConfigurationManager(userSetting, dialogManager, 1)),
                    dialogManager(dialogProvider), userSetting(userSettingProvider)
                {
                    container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                    container.dialogManager = std::make_unique<DialogManager>(dialogProvider);
                }

                NiceMock<MockDialogProvider> dialogProvider;
                NiceMock<MockWinApi> windows;
                NiceMock<MockUserSettingProviderInterface> userSettingProvider;
                PersistenceContainer container;
                RadarRenderableCollection renderables;
                ConfigurableDisplayCollection configurables;
                GdiplusBrushes brushes;
                AsrEventHandlerCollection asrEvents;
                FunctionCallEventHandler functions;
                CountdownTimer countdown;
                UserSetting userSetting;
                DialogManager dialogManager;
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
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(CountdownModuleTest, BootstrapPluginAddsDialog)
        {
            CountdownModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_TIMER_CONFIGURATION));
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToRendererCollection)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenSetsRenderPhaseToBeforeTags)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(1, renderables.CountRenderersInPhase(renderables.beforeTags));
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenRegistersScreenObject)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(3, renderables.CountScreenObjects());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToFunctionCallCollection)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(1, functions.CountCallbacks());
            EXPECT_EQ(0, functions.CountTagFunctions());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(2, configurables.CountDisplays());
        }

        TEST_F(CountdownModuleTest, BootstrapRadarScreenAddsToasrEventsEvents)
        {
            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                configManager,
                renderables,
                configurables,
                brushes,
                asrEvents
            );
            EXPECT_EQ(1, asrEvents.CountHandlers());
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
