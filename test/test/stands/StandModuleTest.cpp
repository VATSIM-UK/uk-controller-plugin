#include "pch/pch.h"
#include "stands/StandModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagItemCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/ExternalMessageEventHandler.h"

using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Return;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Stands::BootstrapPlugin;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandModuleTest : public Test
        {
            public:
                StandModuleTest()
                {
                    container.pushEventProcessors.reset(new PushEventProcessorCollection);
                    container.tagHandler.reset(new TagItemCollection);
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                    container.externalEventHandler.reset(new ExternalMessageEventHandler(true));

                    nlohmann::json gatwick = nlohmann::json::array();
                    gatwick.push_back(
                        {
                            {"id", 1},
                            {"identifier", "31R"},
                        }
                    );
                    gatwick.push_back(
                        {
                            {"id", 2},
                            {"identifier", "35"},
                        }
                    );
                    nlohmann::json heathrow = nlohmann::json::array();
                    heathrow.push_back(
                        {
                            {"id", 3},
                            {"identifier", "76R"},
                        }
                    );
                    nlohmann::json dependency = {
                        {"EGKK", gatwick},
                        {"EGLL", heathrow}
                    };

                    EXPECT_CALL(
                        this->dependencyLoader,
                        LoadDependency("DEPENDENCY_STANDS", nlohmann::json::object())
                    )
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
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(StandModuleTest, ItRegistersTheStandAssignmentEditBoxFunction)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9008));
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }

        TEST_F(StandModuleTest, ItRegistersForExternalMessages)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            EXPECT_EQ(1, this->container.externalEventHandler->CountHandlers());
        }
    }  // namespace Stands
}  // namespace UKControllerPluginTest
