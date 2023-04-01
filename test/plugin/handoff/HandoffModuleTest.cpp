#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "handoff/HandoffModule.h"
#include "integration/IntegrationServer.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "tag/TagItemCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
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
            this->container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
            this->container.runwayDialogEventHandlers = std::make_unique<RunwayDialogAwareCollection>();
            this->container.integrationModuleContainer =
                std::make_unique<IntegrationPersistenceContainer>(nullptr, nullptr, nullptr, nullptr);
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

    TEST_F(HandoffModuleTest, TestItRegistersRunwayDialogHandler)
    {
        BootstrapPlugin(this->container, this->dependencyLoader);
        ASSERT_EQ(1, this->container.runwayDialogEventHandlers->CountHandlers());
    }
} // namespace UKControllerPluginTest::Handoff
