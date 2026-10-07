#include "AircraftStateEventHandler.h"
#include "AircraftStateModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::AircraftState {

    const int FLUSH_PENDING_UPDATES_FREQUENCY = 60;

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        const auto handler =
            std::make_shared<AircraftStateEventHandler>(*container.api, *container.taskRunner, *container.plugin);

        container.flightplanHandler->RegisterHandler(handler);
        container.pushEventProcessors->AddProcessor(handler);
        container.timedHandler->RegisterEvent(handler, FLUSH_PENDING_UPDATES_FREQUENCY);
    }
} // namespace UKControllerPlugin::AircraftState
