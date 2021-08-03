#include "pch/pch.h"
#include "handoff/HandoffModule.h"
#include "mock/MockDependencyLoader.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagItemCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "integration/IntegrationPersistenceContainer.h"

using UKControllerPlugin::Handoff::BootstrapPlugin;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Integration::IntegrationPersistenceContainer;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Handoff {

        class HandoffModuleTest : public Test
        {
            public:
                HandoffModuleTest()
                {
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.activeCallsigns.reset(new ActiveCallsignCollection);
                    this->container.integrationModuleContainer.reset(new IntegrationPersistenceContainer);
                }

                PersistenceContainer container;
                NiceMock<MockDependencyLoader> dependencyLoader;
        };

        TEST_F(HandoffModuleTest, TestItCreatesHandoffCollection)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            ASSERT_EQ(0, this->container.handoffs->CountHandoffs());
        }

        TEST_F(HandoffModuleTest, TestItRegistersTagItem)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            ASSERT_EQ(1, this->container.tagHandler->CountHandlers());
            ASSERT_TRUE(this->container.tagHandler->HasHandlerForItemId(107));
        }

        TEST_F(HandoffModuleTest, TestItRegistersFlightplanHandler)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            ASSERT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(HandoffModuleTest, TestItRegistersActiveCallsignHandler)
        {
            BootstrapPlugin(this->container, this->dependencyLoader);
            ASSERT_EQ(1, this->container.activeCallsigns->CountHandlers());
        }
    }  // namespace Handoff
}  // namespace UKControllerPluginTest
