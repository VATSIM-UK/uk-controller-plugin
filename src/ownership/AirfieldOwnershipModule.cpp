#include "pch/stdafx.h"
#include "ownership/AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "ownership/AirfieldOwnershipHandler.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "dependency/DependencyLoaderInterface.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/ControllerPositionCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Ownership::AirfieldOwnershipHandler;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Ownership {

        void AirfieldOwnershipModule::BootstrapPlugin(
            PersistenceContainer & persistence,
            DependencyLoaderInterface& dependency
        ) {
            persistence.airfieldOwnership.reset(
                new AirfieldOwnershipManager(*persistence.airfields, *persistence.activeCallsigns)
            );

            std::shared_ptr<AirfieldOwnershipHandler> airfieldOwnership(
                new AirfieldOwnershipHandler(
                    *persistence.airfieldOwnership,
                    *persistence.userMessager
                )
            );

            // Add the handlers to the collections.
            persistence.activeCallsigns->AddHandler(airfieldOwnership);
            persistence.commandHandlers->RegisterHandler(airfieldOwnership);
        }
    }  // namespace Ownership
}  // namespace UKControllerPlugin
