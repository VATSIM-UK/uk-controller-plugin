#include "FlightplanStorageBootstrap.h"
#include "StoredFlightplanEventHandler.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::StoredFlightplanEventHandler;

namespace UKControllerPlugin::Flightplan {

    /*
        Bootstraps the event handler surrounding storage of flightplans.
    */
    void FlightplanStorageBootstrap::BootstrapPlugin(const PersistenceContainer& container)
    {
        std::shared_ptr<StoredFlightplanEventHandler> handler =
            std::make_shared<StoredFlightplanEventHandler>(*container.flightplans);

        container.flightplanHandler->RegisterHandler(handler);
        container.timedHandler->RegisterEvent(handler, FlightplanStorageBootstrap::timedEventFrequency);
    }
} // namespace UKControllerPlugin::Flightplan
