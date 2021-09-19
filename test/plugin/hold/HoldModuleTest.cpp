#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"
#include "hold/HoldingData.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "command/CommandHandlerCollection.h"
#include "tag/TagItemCollection.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "hold/CompareHolds.h"
#include "push/PushEventProcessorCollection.h"
#include "api/ApiException.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "hold/HoldManager.h"
#include "hold/PublishedHoldCollection.h"
#include "hold/HoldSelectionMenu.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Hold::BootstrapPlugin;
using UKControllerPlugin::Hold::BootstrapRadarScreen;
using UKControllerPlugin::Hold::CompareHolds;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::Hold {

    class HoldModuleTest : public Test
    {
        public:
        HoldModuleTest() : containerApi(new NiceMock<MockApiInterface>)
        {
            nlohmann::json hold;
            hold = {
                {"id", 1},
                {"fix", "TIMBA"},
                {"description", "TIMBA"},
                {"minimum_altitude", 7000},
                {"maximum_altitude", 15000},
                {"inbound_heading", 309},
                {"turn_direction", "right"},
                {"restrictions", nlohmann::json::array()},
                {"deemed_separated_holds", nlohmann::json::array()}};

            nlohmann::json hold2;
            hold2 = {
                {"id", 2},
                {"fix", "WILLO"},
                {"description", "WILLO"},
                {"minimum_altitude", 7000},
                {"maximum_altitude", 15000},
                {"inbound_heading", 309},
                {"turn_direction", "right"},
                {"restrictions", nlohmann::json::array()},
                {"deemed_separated_holds", nlohmann::json::array()}};

            nlohmann::json profile1;
            profile1["id"] = 1;
            profile1["name"] = "Gatwick Director";
            profile1["holds"] = {1, 2, 3};
            nlohmann::json profile2;
            profile2["id"] = 2;
            profile2["name"] = "Heathrow Director";
            profile2["holds"] = {1, 2, 3};

            ON_CALL(this->mockDependencyProvider, LoadDependency("DEPENDENCY_HOLDS", nlohmann::json::array()))
                .WillByDefault(Return(nlohmann::json::array({hold, hold2})));

            ON_CALL(this->mockDependencyProvider, LoadDependency("DEPENDENCY_HOLD_PROFILE", nlohmann::json::array()))
                .WillByDefault(Return(nlohmann::json::array({profile1, profile2})));

            this->container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            this->container.timedHandler = std::make_unique<TimedEventCollection>();
            this->container.commandHandlers = std::make_unique<CommandHandlerCollection>();
            this->container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            this->container.windows = std::make_unique<NiceMock<MockWinApi>>();
            this->container.tagHandler = std::make_unique<TagItemCollection>();
            this->container.dialogManager = std::make_unique<DialogManager>(this->mockDialogProvider);
            this->container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();
            this->container.taskRunner = std::make_unique<NiceMock<MockTaskRunnerInterface>>();

            this->container.api.reset(this->containerApi);
        }

        ~HoldModuleTest() override
        {
            (void)this->container.api.release();
            delete this->containerApi;
        }

        NiceMock<MockApiInterface>* containerApi;
        NiceMock<MockDialogProvider> mockDialogProvider;
        PersistenceContainer container;
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
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(1, this->container.tagHandler->CountHandlers());
    }

    TEST_F(HoldModuleTest, ItAddsToTimedHandler)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
        EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(7));
    }

    TEST_F(HoldModuleTest, ItAddsToFunctionHandlers)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
    }

    TEST_F(HoldModuleTest, ItAddsHoldSelectionCallback)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9003));
    }

    TEST_F(HoldModuleTest, ItInitialisesHoldManager)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(0, this->container.holdManager->CountHoldingAircraft());
    }

    TEST_F(HoldModuleTest, ItAddsToWebsocketHandlers)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(1, this->container.pushEventProcessors->CountProcessorsForChannel("private-hold-assignments"));
    }

    TEST_F(HoldModuleTest, ItInitialisesHoldDisplayFactory)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_TRUE(this->container.holdDisplayFactory);
    }

    TEST_F(HoldModuleTest, ItRegistersHoldConfigurationDialog)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
        EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_HOLD_SELECTION));
    }

    TEST_F(HoldModuleTest, ItRegistersHoldParametersDialog)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        EXPECT_EQ(2, this->container.dialogManager->CountDialogs());
        EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_HOLD_PARAMS));
    }

    TEST_F(HoldModuleTest, ItLoadsHoldData)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);
        HoldingData expectedHold = {1, "TIMBA", "TIMBA", 7000, 15000, 309, HoldingData::TURN_DIRECTION_RIGHT, {}, {}};

        std::set<const HoldingData*> expectedHoldSet;
        expectedHoldSet.emplace(&expectedHold);

        std::set<const HoldingData*> holds = this->container.publishedHolds->GetForFix("TIMBA");
        EXPECT_EQ(1, holds.size());
        EXPECT_EQ(expectedHold, **holds.cbegin());
    }

    TEST_F(HoldModuleTest, ItLoadsAssignedHolds)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back({{"callsign", "BAW123"}, {"navaid", "TIMBA"}});
        responseData.push_back({{"callsign", "EZY234"}, {"navaid", "WILLO"}});

        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ("TIMBA", this->container.holdManager->GetHoldingAircraft("BAW123")->GetAssignedHold());
        EXPECT_EQ("WILLO", this->container.holdManager->GetHoldingAircraft("EZY234")->GetAssignedHold());
        EXPECT_EQ(2, this->container.holdManager->CountHoldingAircraft());
    }

    TEST_F(HoldModuleTest, ItHandlesApiExceptionsOnAssignedHoldLoad)
    {
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Throw(ApiException("Test")));

        EXPECT_NO_THROW(BootstrapPlugin(this->mockDependencyProvider, this->container));
        EXPECT_EQ(0, this->container.holdManager->CountHoldingAircraft());
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfCallsignMissing)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back({{"navaid", "TIMBA"}});
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfCallsignNotString)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back({{"callsign", 123}, {"navaid", "TIMBA"}});
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfNavaidMissing)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back({
            {"callsign", "BAW123"},
        });
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfNavaidNotString)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back({{"callsign", "BAW123"}, {"navaid", 123}});
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfDataNotObject)
    {
        nlohmann::json responseData = nlohmann::json::array();
        responseData.push_back(nlohmann::json::array({{"callsign", "BAW123", "navaid", "TIMBA"}}));
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, ItDoesntLoadAssignedHoldsIfResponseNotArray)
    {
        nlohmann::json responseData = nlohmann::json::object();
        responseData["BAW123"] = {{"callsign", "BAW123"}, {"navaid", "TIMBA"}};
        ON_CALL(*this->containerApi, GetAssignedHolds()).WillByDefault(Return(responseData));

        BootstrapPlugin(this->mockDependencyProvider, this->container);

        EXPECT_EQ(
            this->container.holdManager->invalidAircraft, this->container.holdManager->GetHoldingAircraft("BAW123"));
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersForAsrEvents)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(2, this->asrEvents.CountHandlers());
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersDisplayManager)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(1, this->container.holdSelectionMenu->CountDisplayManagers());
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersRadarRenderable)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(1, this->radarRenderables.CountRenderers());
        EXPECT_EQ(1, this->radarRenderables.CountRenderersInPhase(this->radarRenderables.afterTags));
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersMenuConfigurationItems)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(2, this->configurableDisplays.CountDisplays());
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersCallbacksWithPluginFunctions)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        size_t countBefore = this->container.pluginFunctionHandlers->CountCallbacks();
        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(countBefore + 2, this->container.pluginFunctionHandlers->CountCallbacks());
    }

    TEST_F(HoldModuleTest, RadarScreenRegistersRadarScreenCommandHandler)
    {
        BootstrapPlugin(this->mockDependencyProvider, this->container);

        BootstrapRadarScreen(
            this->configurableDisplays,
            this->radarRenderables,
            this->asrEvents,
            this->radarScreenCommands,
            this->container);

        EXPECT_EQ(1, this->radarScreenCommands.CountHandlers());
        EXPECT_TRUE(this->radarScreenCommands.ProcessCommand(".ukcp hold"));
    }
} // namespace UKControllerPluginTest::Hold
