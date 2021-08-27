#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/IntegrationServer.h"
#include "intention/IntentionCodeModule.h"
#include "tag/TagItemCollection.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeModule;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPluginTest::IntentionCode {

    class IntentionCodeModuleTest : public Test
    {
        public:
        IntentionCodeModuleTest()
        {
            this->container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            this->container.tagHandler = std::make_unique<TagItemCollection>();
            this->container.controllerHandler = std::make_unique<ControllerStatusEventHandlerCollection>();
            this->container.integrationModuleContainer =
                std::make_unique<UKControllerPlugin::Integration::IntegrationPersistenceContainer>(

                    nullptr, nullptr, nullptr

                );
        }

        PersistenceContainer container;
    };

    TEST_F(IntentionCodeModuleTest, BootstrapPluginRegistersFlightplanEvents)
    {
        IntentionCodeModule::BootstrapPlugin(this->container);

        EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
    }

    TEST_F(IntentionCodeModuleTest, BootstrapPluginRegistersCorrectTagItemEvent)
    {
        IntentionCodeModule::BootstrapPlugin(this->container);

        EXPECT_EQ(1, this->container.tagHandler->CountHandlers());
        EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(IntentionCodeModule::tagItemId));
    }

    TEST_F(IntentionCodeModuleTest, BootstrapPluginRegistersForControllerUpdates)
    {
        IntentionCodeModule::BootstrapPlugin(this->container);

        EXPECT_EQ(1, this->container.controllerHandler->CountHandlers());
    }
} // namespace UKControllerPluginTest::IntentionCode
