#include "pch/pch.h"
#include "flightplan/FlightplanStorageBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"

using UKControllerPlugin::Flightplan::FlightplanStorageBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest {
    namespace Flightplan {

        TEST(FlightplanStorageBootstrap, BootstrapPluginAddsHandlerToTimedEvents)
        {
            PersistenceContainer container;
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();

            FlightplanStorageBootstrap::BootstrapPlugin(container);
            EXPECT_EQ(1, container.timedHandler->CountHandlers());
            EXPECT_EQ(
                1,
                container.timedHandler->CountHandlersForFrequency(FlightplanStorageBootstrap::timedEventFrequency)
            );
        }

        TEST(FlightplanStorageBootstrap, BootstrapPluginAddsHandlerToFlightplanEvents)
        {
            PersistenceContainer container;
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();

            FlightplanStorageBootstrap::BootstrapPlugin(container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
            EXPECT_EQ(
                1,
                container.timedHandler->CountHandlersForFrequency(FlightplanStorageBootstrap::timedEventFrequency)
            );
        }
    }  // namespace Flightplan
} // namespace UKControllerPluginTest
