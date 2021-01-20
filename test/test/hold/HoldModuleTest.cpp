#include "pch/pch.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "timedevent/TimedEventCollection.h"
#include "hold/HoldingData.h"
#include "message/UserMessager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "command/CommandHandlerCollection.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "tag/TagItemCollection.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "mock/MockDependencyLoader.h"
#include "dialog/DialogData.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "hold/CompareHolds.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "mock/MockTaskRunnerInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Hold::BootstrapPlugin;
using UKControllerPlugin::Hold::BootstrapRadarScreen;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::CompareHolds;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Websocket::WebsocketEventProcessorCollection;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldModuleTest : public Test
        {
            public:
                HoldModuleTest()
                    : messager(mockPlugin)
                {
                    nlohmann::json hold;
                    hold = {
                        { "id", 1 },
                        { "fix", "TIMBA" },
                        { "description", "TIMBA" },
                        { "minimum_altitude", 7000 },
                        { "maximum_altitude", 15000 },
                        { "inbound_heading", 309 },
                        { "turn_direction", "right" },
                        { "restrictions", nlohmann::json::array()}
                    };

                    nlohmann::json hold2;
                    hold2 = {
                        { "id", 2 },
                        { "fix", "WILLO" },
                        { "description", "WILLO" },
                        { "minimum_altitude", 7000 },
                        { "maximum_altitude", 15000 },
                        { "inbound_heading", 309 },
                        { "turn_direction", "right" },
                        { "restrictions", nlohmann::json::array()}
                    };

                    nlohmann::json profile1;
                    profile1["id"] = 1;
                    profile1["name"] = "Gatwick Director";
                    profile1["holds"] = { 1, 2, 3 };
                    nlohmann::json profile2;
                    profile2["id"] = 2;
                    profile2["name"] = "Heathrow Director";
                    profile2["holds"] = { 1, 2, 3 };

                    ON_CALL(
                        this->mockDependencyProvider,
                        LoadDependency("DEPENDENCY_HOLDS", nlohmann::json::array())
                    )
                        .WillByDefault(Return(nlohmann::json::array({ hold, hold2 })));

                    ON_CALL(
                        this->mockDependencyProvider,
                        LoadDependency("DEPENDENCY_HOLD_PROFILE", nlohmann::json::array())
                    )
                        .WillByDefault(Return(nlohmann::json::array({ profile1, profile2 })));

                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.timedHandler.reset(new TimedEventCollection);
                    this->container.commandHandlers.reset(new CommandHandlerCollection);
                    this->container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    this->container.windows.reset(new NiceMock<MockWinApi>);
                    this->container.windows.reset(new NiceMock<MockWinApi>);
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.dialogManager.reset(new DialogManager(this->mockDialogProvider));
                    this->container.websocketProcessors.reset(new WebsocketEventProcessorCollection);
                    this->container.taskRunner.reset(new NiceMock<MockTaskRunnerInterface>);


                    this->containerApi = new NiceMock<MockApiInterface>;
                    this->container.api.reset(this->containerApi);
                }

                ~HoldModuleTest()
                {
                    this->container.api.release();
                    delete this->containerApi;
                }

                NiceMock<MockApiInterface> * containerApi;
                NiceMock<MockDialogProvider> mockDialogProvider;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
                UserMessager messager;
                CommandHandlerCollection radarScreenCommands;
                ConfigurableDisplayCollection configurableDisplays;
                RadarRenderableCollection radarRenderables;
                AsrEventHandlerCollection asrEvents;
                NiceMock<MockApiInterface> mockWebApi;
                NiceMock<MockWinApi> mockWinApi;
                NiceMock<MockDependencyLoader> mockDependencyProvider;
        };

        TEST_F(HoldModuleTest, ItAddsToTagItemhandler)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(1, this->container.tagHandler->CountHandlers());
        }

        TEST_F(HoldModuleTest, ItAddsToTimedHandler)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(7));
        }

        TEST_F(HoldModuleTest, ItAddsToFunctionHandlers)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(HoldModuleTest, ItAddsHoldSelectionCallback)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9003));
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldManager)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(0, this->container.holdManager->CountHoldingAircraft());
        }

        TEST_F(HoldModuleTest, ItAddsToWebsocketHandlers)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(1, this->container.websocketProcessors->CountProcessorsForChannel("private-hold-assignments"));
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldDisplayFactory)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_TRUE(this->container.holdDisplayFactory);
        }

        TEST_F(HoldModuleTest, ItRegistersHoldConfigurationDialog)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_HOLD_SELECTION));
        }

        TEST_F(HoldModuleTest, ItRegistersHoldParametersDialog)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_HOLD_PARAMS));
        }

        TEST_F(HoldModuleTest, ItLoadsHoldData)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);
            HoldingData expectedHold = {
                1,
                "TIMBA",
                "TIMBA",
                7000,
                15000,
                309,
                HoldingData::TURN_DIRECTION_RIGHT,
                {}
            };

            std::set<HoldingData, CompareHolds> expectedHoldSet;
            expectedHoldSet.emplace(std::move(expectedHold));

            EXPECT_EQ(expectedHoldSet, this->container.publishedHolds->Get("TIMBA"));
        }

        TEST_F(HoldModuleTest, ItLoadsAssignedHolds)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back({
                {"callsign", "BAW123"},
                {"navaid", "TIMBA"}
            });
            responseData.push_back({
                {"callsign", "EZY234"},
                {"navaid", "WILLO"}
            });

            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ("TIMBA", this->container.holdManager->GetHoldingAircraft("BAW123")->GetAssignedHold());
            EXPECT_EQ("WILLO", this->container.holdManager->GetHoldingAircraft("EZY234")->GetAssignedHold());
            EXPECT_EQ(2, this->container.holdManager->CountHoldingAircraft());
        }

        TEST_F(HoldModuleTest, ItHandlesApiExceptionsOnAssignedHoldLoad)
        {
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Throw(ApiException("Test")));

            EXPECT_NO_THROW(BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager));
            EXPECT_EQ(0, this->container.holdManager->CountHoldingAircraft());
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfCallsignMissing)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back({
                {"navaid", "TIMBA"}
            });
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfCallsignNotString)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back({
                {"callsign", 123},
                {"navaid", "TIMBA"}
            });
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfNavaidMissing)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back({
                {"callsign", "BAW123"},
            });
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfNavaidNotString)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back({
                {"callsign", "BAW123"},
                {"navaid", 123}
            });
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfDataNotObject)
        {
            nlohmann::json responseData = nlohmann::json::array();
            responseData.push_back(nlohmann::json::array({
                {"callsign", "BAW123", "navaid", "TIMBA"}
            }));
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfResponseNotArray)
        {
            nlohmann::json responseData = nlohmann::json::object();
            responseData["BAW123"] = {
                {"callsign", "BAW123"},
                {"navaid", "TIMBA"}
            };
            ON_CALL(*this->containerApi, GetAssignedHolds())
                .WillByDefault(Return(responseData));

            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            EXPECT_EQ(
                this->container.holdManager->invalidAircraft,
                this->container.holdManager->GetHoldingAircraft("BAW123")
            );
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersForAsrEvents)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(2, this->asrEvents.CountHandlers());
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersDisplayManager)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(1, this->container.holdSelectionMenu->CountDisplayManagers());
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersRadarRenderable)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(1, this->radarRenderables.CountRenderers());
            EXPECT_EQ(1, this->radarRenderables.CountRenderersInPhase(this->radarRenderables.afterTags));
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersMenuConfigurationItems)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(2, this->configurableDisplays.CountDisplays());
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersCallbacksWithPluginFunctions)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            size_t countBefore = this->container.pluginFunctionHandlers->CountCallbacks();
            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(countBefore + 2, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(HoldModuleTest, RadarScreenRegistersRadarScreenCommandHandler)
        {
            BootstrapPlugin(this->mockDependencyProvider, this->container, this->messager);

            BootstrapRadarScreen(
                this->configurableDisplays,
                this->radarRenderables,
                this->asrEvents,
                this->radarScreenCommands,
                this->container
            );

            EXPECT_EQ(1, this->radarScreenCommands.CountHandlers());
            EXPECT_TRUE(this->radarScreenCommands.ProcessCommand(".ukcp hold"));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
