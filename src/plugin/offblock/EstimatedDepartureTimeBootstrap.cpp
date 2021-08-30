#include "EstimatedDepartureTimeBootstrap.h"
#include "EstimatedDepartureTimeEventHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Datablock::EstimatedDepartureTimeEventHandler;

namespace UKControllerPlugin::Datablock {

    /*
        Bootstraps the TAG item for use in the plugin. We need to know when flightplans
        change and also respond to requests for TAG items.
    */
    void EstimatedDepartureTimeBootstrap::BootstrapPlugin(const PersistenceContainer& container)
    {
        std::shared_ptr<EstimatedDepartureTimeEventHandler> handler =
            std::make_shared<EstimatedDepartureTimeEventHandler>(*container.flightplans, *container.timeFormatting);

        container.flightplanHandler->RegisterHandler(handler);
        container.tagHandler->RegisterTagItem(EstimatedDepartureTimeBootstrap::tagItemId, handler);
    }
} // namespace UKControllerPlugin::Datablock
