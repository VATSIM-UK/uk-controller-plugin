#include "pch/pch.h"
#include "offblock/ActualOffBlockTimeBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "offblock/ActualOffBlockTimeEventHandler.h"
#include "datablock/DisplayTime.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Datablock::ActualOffBlockTimeEventHandler;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Bootstraps the TAG item for use in the plugin. We need to know when flightplans
            change and also respond to requests for TAG items.
        */
        void ActualOffBlockTimeBootstrap::BootstrapPlugin(const PersistenceContainer & container)
        {
            std::shared_ptr<ActualOffBlockTimeEventHandler> handler = std::make_shared<ActualOffBlockTimeEventHandler>(
                *container.flightplans,
                *container.timeFormatting
            );

            container.tagHandler->RegisterTagItem(ActualOffBlockTimeBootstrap::tagItemId, handler);
            container.flightplanHandler->RegisterHandler(handler);
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin
