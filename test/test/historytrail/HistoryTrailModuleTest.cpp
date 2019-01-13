#include "pch/pch.h"
#include "historytrail/HistoryTrailModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "historytrail/HistoryTrailRepository.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "mock/MockWinApi.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "command/CommandHandlerCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

using ::testing::StrictMock;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace HistoryTrail {

        TEST(HistoryTrailModule, BootstrapPluginSetsUpTrailRepository)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.radarTargetHandler.reset(new RadarTargetEventHandlerCollection);

            HistoryTrailModule::BootstrapPlugin(container);
            EXPECT_FALSE(container.historyTrails->HasAircraft("BAW123"));
        }

        TEST(HistoryTrailModule, BootstrapPluginAddsToRadarTargetHandlers)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.radarTargetHandler.reset(new RadarTargetEventHandlerCollection);

            HistoryTrailModule::BootstrapPlugin(container);
            EXPECT_EQ(1, container.radarTargetHandler->CountHandlers());
        }

        TEST(HistoryTrailModule, BootstrapPluginAddsToFlightplanHandlers)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.radarTargetHandler.reset(new RadarTargetEventHandlerCollection);

            HistoryTrailModule::BootstrapPlugin(container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToFunctionHandlers)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, functionCalls.CountCallbacks());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToRenderables)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, renderables.CountRenderers());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToRenderablesInBeforeTagsPhase)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;


            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, renderables.CountRenderersInPhase(renderables.beforeTags));
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenRegistersNoScreenObjects)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(0, renderables.CountScreenObjects());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToConfigurables)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, configurables.CountDisplays());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, userSettingEvents.CountHandlers());
        }

        TEST(HistoryTrailModule, BootstrapRadarScreenAddsToCommands)
        {
            FunctionCallEventHandler functionCalls;
            HistoryTrailRepository trails;
            RadarRenderableCollection renderables;
            StrictMock<MockWinApi> windows;
            ConfigurableDisplayCollection configurables;
            AsrEventHandlerCollection userSettingEvents;
            CommandHandlerCollection commands;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            HistoryTrailModule::BootstrapRadarScreen(
                functionCalls,
                trails,
                renderables,
                windows,
                configurables,
                userSettingEvents,
                commands,
                mockPlugin
            );
            EXPECT_EQ(1, commands.CountHandlers());
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
