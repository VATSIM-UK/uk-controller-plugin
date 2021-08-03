#include "pch/pch.h"
#include "flightplan/FlightplanStorageBootstrap.h"
#include "flightplan/StoredFlightplanEventHandler.h"

using UKControllerPlugin::Flightplan::StoredFlightplanEventHandler;

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            Bootstraps the event handler surrounding storage of flightplans.
        */
        void FlightplanStorageBootstrap::BootstrapPlugin(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container
        ) {
            std::shared_ptr<StoredFlightplanEventHandler> handler = std::make_shared<StoredFlightplanEventHandler>(
                *container.flightplans
            );

            container.flightplanHandler->RegisterHandler(handler);
            container.timedHandler->RegisterEvent(handler, FlightplanStorageBootstrap::timedEventFrequency);
        }
    }  // namespace Flightplan
}  // namespace UKControllerPlugin
