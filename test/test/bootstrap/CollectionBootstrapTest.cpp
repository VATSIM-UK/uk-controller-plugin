#include "pch/pch.h"
#include "bootstrap/CollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Bootstrap::CollectionBootstrap;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        TEST(CollectionBootstrap, BootstrapPluginCreatesActiveCallsigns)
        {
            PersistenceContainer container;
            container.timedHandler.reset(new TimedEventCollection);
            DependencyCache dependency;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);

            CollectionBootstrap::BootstrapPlugin(container, dependency);
            EXPECT_NO_THROW(container.activeCallsigns->Flush());
        }

        TEST(CollectionBootstrap, BootstrapPluginCreatesAirfields)
        {
            PersistenceContainer container;
            container.timedHandler.reset(new TimedEventCollection);
            DependencyCache dependency;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);

            CollectionBootstrap::BootstrapPlugin(container, dependency);
            EXPECT_NO_THROW(container.airfields->GetSize());
        }

        TEST(CollectionBootstrap, BootstrapPluginCreatesAirfieldOwnership)
        {
            PersistenceContainer container;
            container.timedHandler.reset(new TimedEventCollection);
            DependencyCache dependency;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);

            CollectionBootstrap::BootstrapPlugin(container, dependency);
            EXPECT_NO_THROW(container.airfieldOwnership->Flush());
        }

        TEST(CollectionBootstrap, BootstrapPluginCreatesFlightplans)
        {
            PersistenceContainer container;
            container.timedHandler.reset(new TimedEventCollection);
            DependencyCache dependency;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);

            CollectionBootstrap::BootstrapPlugin(container, dependency);
            EXPECT_NO_THROW(container.flightplans->cend());
        }

        TEST(CollectionBootstrap, BootstrapPluginCreatesMetars)
        {
            PersistenceContainer container;
            container.timedHandler.reset(new TimedEventCollection);
            DependencyCache dependency;
            container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);

            CollectionBootstrap::BootstrapPlugin(container, dependency);
            EXPECT_NO_THROW(container.metarEventHandler->CountHandlers());
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
