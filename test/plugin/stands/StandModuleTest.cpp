#include "bootstrap/PersistenceContainer.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/ExternalMessageEventHandler.h"
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/IntegrationServer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "stands/StandModule.h"
#include "tag/TagItemCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;
using UKControllerPlugin::Integration::InboundIntegrationMessageHandler;
using UKControllerPlugin::Integration::IntegrationPersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::Stands::BootstrapPlugin;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Stands {

    class StandModuleTest : public Test
    {
        public:
        StandModuleTest()
        {
            container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();
            container.tagHandler = std::make_unique<TagItemCollection>();
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            container.externalEventHandler = std::make_shared<ExternalMessageEventHandler>(true);
            container.integrationModuleContainer =
                std::make_unique<IntegrationPersistenceContainer>(nullptr, nullptr, nullptr, nullptr);
            container.integrationModuleContainer->inboundMessageHandler =
                std::make_shared<InboundIntegrationMessageHandler>(nullptr);

            nlohmann::json gatwick = nlohmann::json::array();
            gatwick.push_back({
                {"id", 1},
                {"identifier", "31R"},
            });
            gatwick.push_back({
                {"id", 2},
                {"identifier", "35"},
            });
            nlohmann::json heathrow = nlohmann::json::array();
            heathrow.push_back({
                {"id", 3},
                {"identifier", "76R"},
            });
            nlohmann::json dependency = {{"EGKK", gatwick}, {"EGLL", heathrow}};

            EXPECT_CALL(this->dependencyLoader, LoadDependency("DEPENDENCY_STANDS", nlohmann::json::object()))
                .Times(1)
                .WillOnce(Return(dependency));
        }

        NiceMock<MockDependencyLoader> dependencyLoader;
        PersistenceContainer container;
    };

    TEST_F(StandModuleTest, ItRegistersAssignedStandsTagItem)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(110));
    }

    TEST_F(StandModuleTest, ItRegistersForWebsocketEvents)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_EQ(1, this->container.pushEventProcessors->CountProcessorsForChannel("private-stand-assignments"));
    }

    TEST_F(StandModuleTest, ItRegistersForFlightplanEvents)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
    }

    TEST_F(StandModuleTest, ItRegistersTheStandAssignmentPopupMenuFunction)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9007));
        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackByDescription("Stand Selected"));
    }

    TEST_F(StandModuleTest, ItRegistersTheStandAssignmentEditBoxFunction)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9008));
        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
    }

    TEST_F(StandModuleTest, ItRegistersForExternalMessages)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_EQ(1, this->container.externalEventHandler->CountHandlers());
    }

    TEST_F(StandModuleTest, ItRegistersForIntegrationMessages)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        EXPECT_EQ(2, container.integrationModuleContainer->inboundMessageHandler->CountProcessors());
    }
} // namespace UKControllerPluginTest::Stands
