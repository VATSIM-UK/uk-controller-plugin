#include "ActualOffBlockTimeBootstrap.h"
#include "ActualOffBlockTimeEventHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "datablock/DisplayTime.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Datablock::ActualOffBlockTimeEventHandler;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPlugin::Datablock {

    /*
        Bootstraps the TAG item for use in the plugin. We need to know when flightplans
        change and also respond to requests for TAG items.
    */
    void ActualOffBlockTimeBootstrap::BootstrapPlugin(const PersistenceContainer& container)
    {
        std::shared_ptr<ActualOffBlockTimeEventHandler> handler =
            std::make_shared<ActualOffBlockTimeEventHandler>(*container.flightplans, *container.timeFormatting);

        container.tagHandler->RegisterTagItem(ActualOffBlockTimeBootstrap::tagItemId, handler);
        container.flightplanHandler->RegisterHandler(handler);
    }
} // namespace UKControllerPlugin::Datablock
