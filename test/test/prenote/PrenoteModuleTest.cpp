#include "pch/pch.h"
#include "prenote/PrenoteModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "dependency/DependencyCache.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteModuleTest : public Test
        {
            public:
                void SetUp(void)
                {
                    container.controllerPositions.reset(new ControllerPositionCollection);
                    container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_GND", 121.800, "GND", { "EGKK" }))
                    );
                    container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_TWR", 124.220, "TWR", { "EGKK" }))
                    );
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.userMessager.reset(new UserMessager(this->mockPlugin));
                };

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
        };

        TEST_F(PrenoteModuleTest, BootstrapPluginDoesNothingIfNoDependency)
        {
            DependencyCache cache;
            PrenoteModule::BootstrapPlugin(container, cache);
            EXPECT_EQ(0, container.flightplanHandler->CountHandlers());
        }

        TEST_F(PrenoteModuleTest, InformsUserIfPrenotesDataNotFound)
        {
            DependencyCache cache;
            EXPECT_CALL(
                    mockPlugin,
                    ChatAreaMessage(
                        BootstrapWarningMessage::handler,
                        BootstrapWarningMessage::sender,
                        "Prenote data file not found, prenotes not loaded",
                        true,
                        true,
                        true,
                        true,
                        true
                    )
                )
                .Times(1);

            PrenoteModule::BootstrapPlugin(container, cache);
        }

        TEST_F(PrenoteModuleTest, DoesNotAddHandlerIfPrenotesInvalidJson)
        {
            DependencyCache cache;
            cache.AddDependency(PrenoteModule::dependencyFile, "[abcs]");

            PrenoteModule::BootstrapPlugin(container, cache);
            EXPECT_EQ(0, container.flightplanHandler->CountHandlers());
        }

        TEST_F(PrenoteModuleTest, InformsUserIfPrenotesInvalidJson)
        {
            DependencyCache cache;
            cache.AddDependency(PrenoteModule::dependencyFile, "[abcs]");

            EXPECT_CALL(
                    mockPlugin,
                    ChatAreaMessage(
                        BootstrapWarningMessage::handler,
                        BootstrapWarningMessage::sender,
                        "JSON exception when parsing prenotes file, prenotes not loaded",
                        true,
                        true,
                        true,
                        true,
                        true
                    )
                )
                .Times(1);

            PrenoteModule::BootstrapPlugin(container, cache);
        }

        TEST_F(PrenoteModuleTest, DoesNothingIfErrorParsingPrenotes)
        {
            DependencyCache cache;
            cache.AddDependency(PrenoteModule::dependencyFile, "{}");

            PrenoteModule::BootstrapPlugin(container, cache);
            EXPECT_EQ(0, container.flightplanHandler->CountHandlers());
        }

        TEST_F(PrenoteModuleTest, BootstrapPluginAddsHandlerToFlightplanEvents)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = { "EGKK_GND", "EGKK_TWR" };

            DependencyCache cache;
            cache.AddDependency(PrenoteModule::dependencyFile, json.dump());

            PrenoteModule::BootstrapPlugin(container, cache);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
