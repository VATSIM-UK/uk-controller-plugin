#include "pch/pch.h"
#include "intention/IntentionCodeModule.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::IntentionCode::IntentionCodeModule;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest {
    namespace IntentionCode {


        TEST(IntentionCodeModule, BootstrapPluginRegistersFlightplanEvents)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.tagHandler.reset(new TagItemCollection);

            IntentionCodeModule::BootstrapPlugin(container);

            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }

        TEST(IntentionCodeModule, BootstrapPluginRegistersCorrectTagItemEvent)
        {
            PersistenceContainer container;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            container.tagHandler.reset(new TagItemCollection);

            IntentionCodeModule::BootstrapPlugin(container);

            EXPECT_EQ(1, container.tagHandler->CountHandlers());
            EXPECT_TRUE(container.tagHandler->HasHandlerForItemId(IntentionCodeModule::tagItemId));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
