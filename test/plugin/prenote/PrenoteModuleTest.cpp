#include "pch/pch.h"
#include "prenote/PrenoteModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "mock/MockDependencyLoader.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteModuleTest : public Test
        {
            public:
                void SetUp(void)
                {
                    container.controllerPositions.reset(new ControllerPositionCollection);
                    container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(1, "EGKK_GND", 121.800, {"EGKK"}, true, false))
                    );
                    container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(2, "EGKK_TWR", 124.220, {"EGKK"}, true, false))
                    );
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.userMessager.reset(new UserMessager(this->mockPlugin));
                    container.tagHandler = std::make_unique<UKControllerPlugin::Tag::TagItemCollection>();
                };

                NiceMock<MockDependencyLoader> dependency;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                PersistenceContainer container;
        };

        TEST_F(PrenoteModuleTest, BootstrapPluginDoesNothingIfNoDependency)
        {
            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_EQ(0, container.flightplanHandler->CountHandlers());
        }

        TEST_F(PrenoteModuleTest, InformsUserIfPrenotesDataNotFound)
        {
            EXPECT_CALL(
                    mockPlugin,
                    ChatAreaMessage(
                        BootstrapWarningMessage::handler,
                        BootstrapWarningMessage::sender,
                        "Prenote data not found, prenotes not loaded",
                        true,
                        true,
                        true,
                        true,
                        true
                    )
                )
                .Times(1);

            PrenoteModule::BootstrapPlugin(container, dependency);
        }


        TEST_F(PrenoteModuleTest, DoesNothingIfErrorParsingPrenotes)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return("{}"_json));

            PrenoteModule::BootstrapPlugin(container, dependency);
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

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }
        
        TEST_F(PrenoteModuleTest, ItRegistersStatusIndicatorTagItem)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = { "EGKK_GND", "EGKK_TWR" };
            
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));
            
            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(127));
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
