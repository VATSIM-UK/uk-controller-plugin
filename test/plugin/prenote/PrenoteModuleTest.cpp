#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"
#include "mock/MockDependencyLoader.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagItemCollection.h"
#include "prenote/PrenoteModule.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest::Prenote {

    class PrenoteModuleTest : public Test
    {
        public:
        void SetUp() override
        {
            PushEventProcessorCollection test;
            test.CountProcessorsForAll();
            container.controllerPositions = std::make_unique<ControllerPositionCollection>();
            container.controllerPositions->AddPosition(std::make_unique<ControllerPosition>(
                1, "EGKK_GND", 121.800, std::vector<std::string>{"EGKK"}, true, false));
            container.controllerPositions->AddPosition(std::make_unique<ControllerPosition>(
                2, "EGKK_TWR", 124.220, std::vector<std::string>{"EGKK"}, true, false));
            this->container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();
            container.userMessager = std::make_unique<UserMessager>(mockPlugin);
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.tagHandler = std::make_unique<UKControllerPlugin::Tag::TagItemCollection>();
            container.pluginFunctionHandlers = std::make_unique<UKControllerPlugin::Plugin::FunctionCallEventHandler>();
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

    TEST_F(PrenoteModuleTest, BootstrapPluginRegistersTheNewPrenoteMessageHandler)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("prenote-message.received"));
    }

    TEST_F(PrenoteModuleTest, BootstrapPluginRegistersTheAcknowledgePrenoteMessageHandler)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("prenote-message.acknowledged"));
    }

    TEST_F(PrenoteModuleTest, BootstrapPluginRegistersTheDeletePrenoteMessageHandler)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("prenote-message.deleted"));
    }

    TEST_F(PrenoteModuleTest, BootstrapPluginRegistersTheTimeoutHandler)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(10));
    }

    TEST_F(PrenoteModuleTest, ItRegistersStatusIndicatorTagItem)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(127));
    }

    TEST_F(PrenoteModuleTest, ItRegistersCancelMenuTagFunction)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9016));
    }

    TEST_F(PrenoteModuleTest, ItRegistersCancelMenuCallback)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Prenote Cancel"));
    }

    TEST_F(PrenoteModuleTest, ItRegistersSendMenuTagFunction)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9017));
    }

    TEST_F(PrenoteModuleTest, ItRegistersSendMenuCallback)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Prenote Send"));
    }
} // namespace UKControllerPluginTest::Prenote
