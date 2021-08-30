#include "EstimatedOffBlockTimeBootstrap.h"
#include "EstimatedOffBlockTimeEventHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeEventHandler;

namespace UKControllerPlugin::Datablock {

    /*
        Bootstraps the TAG item for use in the plugin. We need to know when flightplans
        change and also respond to requests for TAG items.
    */
    void EstimatedOffBlockTimeBootstrap::BootstrapPlugin(const PersistenceContainer& container)
    {
        std::shared_ptr<EstimatedOffBlockTimeEventHandler> handler =
            std::make_shared<EstimatedOffBlockTimeEventHandler>(*container.flightplans, *container.timeFormatting);

        container.tagHandler->RegisterTagItem(EstimatedOffBlockTimeBootstrap::tagItemId, handler);
    }
} // namespace UKControllerPlugin::Datablock
