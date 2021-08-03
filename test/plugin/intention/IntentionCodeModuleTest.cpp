#include "pch/pch.h"
#include "intention/IntentionCodeModule.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "tag/TagItemCollection.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::IntentionCode::IntentionCodeModule;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        class IntentionCodeModuleTest : public Test
        {
            public:
                IntentionCodeModuleTest()
                {
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.controllerHandler.reset(new ControllerStatusEventHandlerCollection);
                    this->container.integrationModuleContainer.reset(
                        new UKControllerPlugin::Integration::IntegrationPersistenceContainer{

                        }
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
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
