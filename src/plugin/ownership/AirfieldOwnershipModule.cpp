#include "AirfieldOwnershipHandler.h"
#include "AirfieldOwnershipManager.h"
#include "AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Ownership {

    void
    AirfieldOwnershipModule::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        persistence.airfieldOwnership =
            std::make_unique<AirfieldOwnershipManager>(*persistence.airfields, *persistence.activeCallsigns);

        std::shared_ptr<AirfieldOwnershipHandler> airfieldOwnership(
            new AirfieldOwnershipHandler(*persistence.airfieldOwnership, *persistence.userMessager));

        // Add the handlers to the collections.
        persistence.activeCallsigns->AddHandler(airfieldOwnership);
        persistence.commandHandlers->RegisterHandler(airfieldOwnership);
    }
} // namespace UKControllerPlugin::Ownership
