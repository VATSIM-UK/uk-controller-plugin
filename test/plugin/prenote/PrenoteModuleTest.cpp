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
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagItemCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest {
    namespace Prenote {

        class PrenoteModuleTest : public Test
        {
            public:
            void SetUp(void)
            {
                container.controllerPositions.reset(new ControllerPositionCollection);
                container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(1, "EGKK_GND", 121.800, {"EGKK"}, true, false)));
                container.controllerPositions->AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(2, "EGKK_TWR", 124.220, {"EGKK"}, true, false)));
                this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                container.userMessager.reset(new UserMessager(this->mockPlugin));
                container.tagHandler = std::make_unique<UKControllerPlugin::Tag::TagItemCollection>();
                container.pluginFunctionHandlers =
                    std::make_unique<UKControllerPlugin::Plugin::FunctionCallEventHandler>();
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
                    true))
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
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

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
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(127));
        }

        TEST_F(PrenoteModuleTest, ItRegistersCancelMenuTagFunction)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9016));
        }

        TEST_F(PrenoteModuleTest, ItRegistersCancelMenuCallback)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Prenote Cancel"));
        }

        TEST_F(PrenoteModuleTest, ItRegistersSendMenuTagFunction)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9017));
        }

        TEST_F(PrenoteModuleTest, ItRegistersSendMenuCallback)
        {
            nlohmann::json json;
            json[0] = {};
            json[0]["type"] = "sid";
            json[0]["airfield"] = "EGKK";
            json[0]["departure"] = "BIG2X";
            json[0]["recipient"] = {"EGKK_GND", "EGKK_TWR"};

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTE", nlohmann::json::array()))
                .WillByDefault(Return(json));

            PrenoteModule::BootstrapPlugin(container, dependency);
            EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Prenote Send"));
        }
    } // namespace Prenote
} // namespace UKControllerPluginTest
