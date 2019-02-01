#include "pch/pch.h"
#include "hold/HoldModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "timedevent/TimedEventCollection.h"
#include "hold/HoldingData.h"
#include "message/UserMessager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Hold::BootstrapPlugin;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Wake {

        class HoldModuleTest : public Test
        {
            public:
                HoldModuleTest()
                    : messager(mockPlugin)
                {
                    nlohmann::json data;
                    data["TIMBA"] = {
                        { "minimum", 7000 },
                        { "maximum", 15000 },
                        { "inbound", 309 },
                        { "direction", "right" }
                    };
                    dependencies.AddDependency("arrival-holds.json", data.dump());

                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.timedHandler.reset(new TimedEventCollection);
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
                DependencyCache dependencies;
                UserMessager messager;
        };

        TEST_F(HoldModuleTest, ItAddsToFlightplanHandler)
        {
            BootstrapPlugin(this->container, this->dependencies, this->messager);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(HoldModuleTest, ItAddsToTimedHandler)
        {
            BootstrapPlugin(this->container, this->dependencies, this->messager);
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(5));
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldManager)
        {
            BootstrapPlugin(this->container, this->dependencies, this->messager);
            EXPECT_FALSE(this->container.holdManager->HasHold("TIMBA"));
        }

        TEST_F(HoldModuleTest, ItInitialisesHoldDataCollection)
        {
            BootstrapPlugin(this->container, this->dependencies, this->messager);
            EXPECT_EQ(1, this->container.holds->Count());
        }

        TEST_F(HoldModuleTest, ItLoadsHoldData)
        {
            BootstrapPlugin(this->container, this->dependencies, this->messager);
            HoldingData expectedHold = {
                "TIMBA",
                7000,
                15000,
                309,
                HoldingData::TURN_DIRECTION_RIGHT
            };

            EXPECT_TRUE(expectedHold == this->container.holds->Get("TIMBA"));
        }

        TEST_F(HoldModuleTest, ItGracefullyHandlesCorruptHoldData)
        {
            DependencyCache newDependencies;
            newDependencies.AddDependency("arrival-holds.json", "{]");
            BootstrapPlugin(this->container, newDependencies, this->messager);
            EXPECT_EQ(0, this->container.holds->Count());
        }

        TEST_F(HoldModuleTest, ItGracefullyHandlesMissingDependency)
        {
            DependencyCache newDependencies;
            BootstrapPlugin(this->container, newDependencies, this->messager);
            EXPECT_EQ(0, this->container.holds->Count());
        }

        TEST_F(HoldModuleTest, ItReportsNoHoldsToTheUser)
        {
            DependencyCache newDependencies;
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

            BootstrapPlugin(this->container, newDependencies, this->messager);
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
