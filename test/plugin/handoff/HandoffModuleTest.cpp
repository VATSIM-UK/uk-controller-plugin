#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "handoff/HandoffModule.h"
#include "handoff/HandoffCollection.h"
#include "integration/IntegrationServer.h"
#include "tag/TagItemCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Handoff::BootstrapPlugin;
using UKControllerPlugin::Integration::IntegrationPersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Handoff {

    class HandoffModuleTest : public Test
    {
        public:
        HandoffModuleTest()
        {
            this->container.tagHandler = std::make_unique<TagItemCollection>();
            this->container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            this->container.activeCallsigns = std::make_unique<ActiveCallsignCollection>();
            this->container.integrationModuleContainer =
                std::make_unique<IntegrationPersistenceContainer>(nullptr, nullptr, nullptr);
        }

        PersistenceContainer container;
        NiceMock<MockDependencyLoader> dependencyLoader;
    };

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
} // namespace UKControllerPluginTest::Handoff
