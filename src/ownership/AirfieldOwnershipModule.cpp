#include "pch/stdafx.h"
#include "ownership/AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "ownership/ControllerAirfieldOwnershipHandler.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "dependency/DependencyLoaderInterface.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/ControllerPositionCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Ownership::ControllerAirfieldOwnershipHandler;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Ownership {

        void AirfieldOwnershipModule::BootstrapPlugin(
            PersistenceContainer & persistence,
            DependencyLoaderInterface& dependency
        ) {
            persistence.controllerPositions = std::move(ControllerPositionCollectionFactory::Create(dependency));

            std::shared_ptr<ControllerAirfieldOwnershipHandler> airfieldOwnership(
                new ControllerAirfieldOwnershipHandler(
                    *persistence.controllerPositions,
                    *persistence.airfieldOwnership,
                    *persistence.activeCallsigns,
                    *persistence.userMessager
                )
            );

            // Add the handlers to the collections.
            persistence.controllerHandler->RegisterHandler(airfieldOwnership);
            persistence.commandHandlers->RegisterHandler(airfieldOwnership);
        }
    }  // namespace Ownership
}  // namespace UKControllerPlugin
