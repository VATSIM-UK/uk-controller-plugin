#include "pch/stdafx.h"
#include "bootstrap/CollectionBootstrap.h"
#include "dependency/DependencyCache.h"
#include "controller/ActiveCallsignCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "airfield/AirfieldCollection.h"
#include "metar/MetarEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Airfield::AirfieldCollectionFactory;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;

namespace UKControllerPlugin {
    namespace Bootstrap {

        void CollectionBootstrap::BootstrapPlugin(PersistenceContainer & persistence, DependencyCache & dependency)
        {
            // Reset resources
            persistence.activeCallsigns.reset(new ActiveCallsignCollection);
            persistence.airfields = std::move(AirfieldCollectionFactory::Create(dependency));
            persistence.airfieldOwnership.reset(
                new AirfieldOwnershipManager(*persistence.airfields, *persistence.activeCallsigns)
            );
            persistence.flightplans.reset(new StoredFlightplanCollection);
            persistence.metarEventHandler.reset(new MetarEventHandlerCollection);
            persistence.commandHandlers.reset(new CommandHandlerCollection);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
