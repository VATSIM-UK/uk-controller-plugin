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
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Hold::BootstrapPlugin;
using UKControllerPlugin::Hold::BootstrapRadarScreen;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Dependency::DependencyCache;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

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
                    this->container.api.reset(new NiceMock<MockApiInterface>);
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
                UserMessager messager;
                ConfigurableDisplayCollection configurableDisplays;
                DependencyCache loadedDependencies;
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

        TEST_F(HoldModuleTest, ItInitialisesHoldManager)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_FALSE(this->container.holdManager->HasHold("TIMBA"));
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldDataCollection)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            EXPECT_EQ(1, this->container.holds->Count());
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

            EXPECT_TRUE(expectedHold == this->container.holds->Get(1));
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

        TEST_F(HoldModuleTest, ItAddsToFunctionHandlers)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            BootstrapRadarScreen(*this->container.pluginFunctionHandlers, this->configurableDisplays);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        }

        TEST_F(HoldModuleTest, ItAddsToConfigurableDisplays)
        {
            BootstrapPlugin(this->loadedDependencies, this->container, this->messager);
            BootstrapRadarScreen(*this->container.pluginFunctionHandlers, this->configurableDisplays);
            EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
