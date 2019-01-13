#include "pch/pch.h"
#include "countdown/CountdownModule.h"
#include "countdown/CountdownTimer.h"
#include "mock/MockWinApi.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/AsrEventHandlerCollection.h"

using UKControllerPlugin::Countdown::CountdownModule;
using UKControllerPlugin::Countdown::CountdownTimer;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Countdown {

        TEST(CountdownModule, BootstrapPluginCreatesObject)
        {
            std::shared_ptr<CountdownTimer> countdownPointer;
            StrictMock<MockWinApi> windows;

            CountdownModule::BootstrapPlugin(countdownPointer, windows);
            EXPECT_EQ(0, countdownPointer->GetSecondsRemaining());
        }

        TEST(CountdownModule, BootstrapRadarScreenAddsToRendererCollection)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST(CountdownModule, BootstrapRadarScreenSetsRenderPhaseToBeforeTags)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(1, renderables.CountRenderersInPhase(renderables.beforeTags));
        }

        TEST(CountdownModule, BootstrapRadarScreenRegistersScreenObject)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(3, renderables.CountScreenObjects());
        }

        TEST(CountdownModule, BootstrapRadarScreenAddsToFunctionCallCollection)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(1, functions.CountCallbacks());
            EXPECT_EQ(0, functions.CountTagFunctions());
        }

        TEST(CountdownModule, BootstrapRadarScreenAddsToConfigurableDisplays)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(1, configurables.CountDisplays());
        }

        TEST(CountdownModule, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            StrictMock<MockWinApi> windows;
            FunctionCallEventHandler functions;
            CountdownTimer countdown(windows);
            RadarRenderableCollection renderables;
            ConfigurableDisplayCollection configurables;
            GdiplusBrushes brushes;
            AsrEventHandlerCollection userSetting;

            CountdownModule::BootstrapRadarScreen(
                functions,
                countdown,
                renderables,
                configurables,
                brushes,
                userSetting
            );
            EXPECT_EQ(1, userSetting.CountHandlers());
        }
    }  // namespace Countdown
}  // namespace UKControllerPluginTest
