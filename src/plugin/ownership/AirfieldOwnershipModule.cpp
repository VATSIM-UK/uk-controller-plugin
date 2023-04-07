#include "AirfieldOwnershipHandler.h"
#include "AirfieldOwnershipManager.h"
#include "AirfieldOwnershipModule.h"
#include "AirfieldServiceProviderCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Ownership {

    std::shared_ptr<AirfieldOwnershipManager> manager; // NOLINT

    void
    AirfieldOwnershipModule::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {

        persistence.airfieldOwnership = std::make_shared<AirfieldServiceProviderCollection>();
        manager = std::make_shared<AirfieldOwnershipManager>(
            persistence.airfieldOwnership, *persistence.airfields, *persistence.activeCallsigns);

        std::shared_ptr<AirfieldOwnershipHandler> airfieldOwnership(
            new AirfieldOwnershipHandler(*manager, *persistence.userMessager));

        // Add the handlers to the collections.
        persistence.activeCallsigns->AddHandler(airfieldOwnership);
        persistence.commandHandlers->RegisterHandler(airfieldOwnership);
    }
} // namespace UKControllerPlugin::Ownership
