#include "pch/pch.h"
#include "historytrail/HistoryTrailModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "historytrail/HistoryTrailRepository.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
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
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

using ::testing::NiceMock;
using ::testing::Test;


namespace UKControllerPluginTest {
    namespace HistoryTrail {

        class HistoryTrailModuleTest : public Test
        {
            public:

                HistoryTrailModuleTest()
                    : dialogManager(mockProvider)
                {
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.radarTargetHandler.reset(new RadarTargetEventHandlerCollection);
                    container.dialogManager.reset(new DialogManager(this->mockProvider));
                }


                PersistenceContainer container;
                FunctionCallEventHandler functionCalls;
                HistoryTrailRepository trails;
                RadarRenderableCollection renderables;
                NiceMock<MockDialogProvider> mockProvider;
                DialogManager dialogManager;
                ConfigurableDisplayCollection configurables;
                AsrEventHandlerCollection userSettingEvents;
                CommandHandlerCollection commands;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        };

        TEST_F(HistoryTrailModuleTest, BootstrapPluginSetsUpTrailRepository)
        {
            HistoryTrailModule::BootstrapPlugin(this->container);
            EXPECT_FALSE(this->container.historyTrails->HasAircraft("BAW123"));
        }

        TEST_F(HistoryTrailModuleTest, BootstrapPluginAddsToRadarTargetHandlers)
        {
            HistoryTrailModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.radarTargetHandler->CountHandlers());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapPluginAddsToFlightplanHandlers)
        {
            HistoryTrailModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapPluginCreatesDialog)
        {
            HistoryTrailModule::BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_HISTORY_TRAIL));
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToFunctionHandlers)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->functionCalls.CountCallbacks());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToRenderables)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->renderables.CountRenderers());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToRenderablesInBeforeTagsPhase)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->renderables.CountRenderersInPhase(renderables.beforeTags));
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenRegistersNoScreenObjects)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(0, this->renderables.CountScreenObjects());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToConfigurables)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->configurables.CountDisplays());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToUserSettingEvents)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->userSettingEvents.CountHandlers());
        }

        TEST_F(HistoryTrailModuleTest, BootstrapRadarScreenAddsToCommands)
        {
            HistoryTrailModule::BootstrapRadarScreen(
                this->functionCalls,
                this->trails,
                this->renderables,
                this->dialogManager,
                this->configurables,
                this->userSettingEvents,
                this->commands,
                this->mockPlugin
            );
            EXPECT_EQ(1, this->commands.CountHandlers());
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
