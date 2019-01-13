#include "pch/pch.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "dependency/DependencyCache.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;

namespace UKControllerPluginTest {
    namespace InitialAltitude {

        TEST(InitialAltitudeModule, BootstrapPluginCreatesInitialAltitudes)
        {
            PersistenceContainer container;
            DependencyCache dependency;
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();

            InitialAltitudeModule::BootstrapPlugin(dependency, container);
            EXPECT_NO_THROW(container.initialAltitudes->HasSid("EGLL", "DET2G"));
        }

        TEST(InitialAltitudeModule, BootstrapPluginRegistersFlightplanEvents)
        {
            PersistenceContainer container;
            DependencyCache dependency;
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();

            InitialAltitudeModule::BootstrapPlugin(dependency, container);
            EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPluginTest
