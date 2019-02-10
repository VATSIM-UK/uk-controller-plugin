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
#include "dependency/DependencyCache.h"
#include "mock/MockApiInterface.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Hold::BootstrapPlugin;
using UKControllerPlugin::Hold::BootstrapRadarScreen;
using UKControllerPlugin::Hold::LoadDependencies;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPluginTest::Api::MockApiInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Wake {

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
                        { "turn_direction", "right" }
                    };
                    loadedDependencies.AddJsonDependency("holds.json", nlohmann::json::array({ hold }));

                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.timedHandler.reset(new TimedEventCollection);
                    this->container.commandHandlers.reset(new CommandHandlerCollection);
                    this->container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    this->container.windows.reset(new NiceMock<MockWinApi>);
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
                UserMessager messager;
                ConfigurableDisplayCollection configurableDisplays;
                DependencyCache loadedDependencies;
                NiceMock<MockApiInterface> mockWebApi;
                NiceMock<MockWinApi> mockWinApi;
        };

        TEST_F(HoldModuleTest, ItAddsToFlightplanHandler)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(HoldModuleTest, ItAddsToTimedHandler)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(5));
        }

        TEST_F(HoldModuleTest, ItAddsToCommandHandlers)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.commandHandlers->CountHandlers());
        }

        TEST_F(HoldModuleTest, ItAddsToFunctionHandlers)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldManager)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.holdManager->CountHolds());
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldDisplays)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(0, this->container.holdWindows->CountDisplays());
        }

        TEST_F(HoldModuleTest, ItLoadsHoldData)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            HoldingData expectedHold = {
                1,
                "TIMBA",
                "TIMBA",
                7000,
                15000,
                309,
                HoldingData::TURN_DIRECTION_RIGHT
            };

            EXPECT_TRUE(expectedHold == this->container.holdManager->GetManagedHold(1)->holdParameters);
        }

        TEST_F(HoldModuleTest, ItReportsNoHoldsToTheUser)
        {
            DependencyCache noDependencies;
            EXPECT_CALL(
                this->mockPlugin,
                ChatAreaMessage(
                    BootstrapWarningMessage::handler,
                    BootstrapWarningMessage::sender,
                    "No holds were loaded for the hold manager",
                    true,
                    true,
                    true,
                    true,
                    true
                )
            )
                .Times(1);

            BootstrapPlugin(noDependencies, this->container, this->messager);
        }

        TEST_F(HoldModuleTest, ItAddsToConfigurableDisplays)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            BootstrapRadarScreen(this->configurableDisplays);
            EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        }

        TEST_F(HoldModuleTest, ItUpdatesAndLoadsDependencies)
        {
            DependencyCache newDependencies;

            nlohmann::json depData;
            depData["foo"] = "bar";

            ON_CALL(this->mockWebApi, GetHoldDependency())
                .WillByDefault(Return(depData));

            EXPECT_CALL(this->mockWinApi, WriteToFile("dependencies/holds.json", depData.dump(), true))
                .Times(1);

            ON_CALL(this->mockWinApi, FileExists("dependencies/holds.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWinApi, ReadFromFileMock("dependencies/holds.json", true))
                .WillByDefault(Return(depData.dump()));

            LoadDependencies(&newDependencies, this->mockWebApi, this->mockWinApi);
            EXPECT_EQ(1, newDependencies.JsonDependencyCount());
            EXPECT_TRUE(depData == newDependencies.GetJsonDependency("holds.json"));
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
