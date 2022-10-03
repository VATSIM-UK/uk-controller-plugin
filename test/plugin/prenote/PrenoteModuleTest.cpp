#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "prenote/PrenoteMessageEventHandlerCollection.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteModule.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteModule;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
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

        AsrEventHandlerCollection asrHandlers;
        ConfigurableDisplayCollection configurableDisplays;
        RadarRenderableCollection radarRenderables;
        NiceMock<MockDependencyLoader> dependency;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        PersistenceContainer container;
    };

    TEST_F(PrenoteModuleTest, BootstrapPluginAddsHandlerToFlightplanEvents)
    {
        ON_CALL(this->dependency, LoadDependency("DEPENDENCY_PRENOTES_V2", nlohmann::json::array()))
            .WillByDefault(Return(nlohmann::json::array()));

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

    TEST_F(PrenoteModuleTest, ItRegistersStatusViewTagFunction)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9018));
    }

    TEST_F(PrenoteModuleTest, ItRegistersAcknowledgeTagFunction)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9019));
    }

    TEST_F(PrenoteModuleTest, ItRegistersPrenoteCollectionWithContainer)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(0, container.prenotes->Count());
    }

    TEST_F(PrenoteModuleTest, ItRegistersPrenoteMessageEventHandlerCollectionWithContainer)
    {
        PrenoteModule::BootstrapPlugin(container, dependency);
        EXPECT_EQ(4, container.prenoteMessageHandlers->CountHandlers());
    }

    TEST_F(PrenoteModuleTest, ItRegistersRenderables)
    {
        PrenoteModule::BootstrapRadarScreen(container, radarRenderables);
        EXPECT_EQ(1, radarRenderables.CountRenderers());
        EXPECT_EQ(1, radarRenderables.CountRenderersInPhase(radarRenderables.afterLists));
    }
} // namespace UKControllerPluginTest::Prenote
