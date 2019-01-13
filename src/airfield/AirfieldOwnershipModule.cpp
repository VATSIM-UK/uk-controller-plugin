#include "pch/stdafx.h"
#include "airfield/AirfieldOwnershipModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "massevent/MassEvent.h"
#include "airfield/ControllerAirfieldOwnershipHandler.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "dependency/DependencyCache.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/ControllerPositionCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::EventHandler::MassEvent;
using UKControllerPlugin::Airfield::ControllerAirfieldOwnershipHandler;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyCache;

namespace UKControllerPlugin {
    namespace Airfield {

        void AirfieldOwnershipModule::BootstrapPlugin(
            PersistenceContainer & persistence,
            const DependencyCache & dependency
        ) {
            MassEvent mass(
                *persistence.plugin,
                persistence.initialAltitudeEvents,
                *persistence.flightplans,
                persistence.squawkEvents
            );
            persistence.controllerPositions = std::move(ControllerPositionCollectionFactory::Create(dependency));

            std::shared_ptr<ControllerAirfieldOwnershipHandler> airfieldOwnership(
                new ControllerAirfieldOwnershipHandler(
                    *persistence.controllerPositions,
                    *persistence.airfieldOwnership,
                    *persistence.activeCallsigns,
                    mass,
                    *persistence.userMessager
                )
            );

            // Add the handlers to the collections.
            persistence.controllerHandler->RegisterHandler(airfieldOwnership);
            persistence.commandHandlers->RegisterHandler(airfieldOwnership);
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
