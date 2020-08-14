#include "pch/stdafx.h"
#include "bootstrap/CollectionBootstrap.h"
#include "dependency/DependencyLoaderInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "airfield/AirfieldCollection.h"
#include "metar/MetarEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "command/CommandHandlerCollection.h"
#include "controller/HandoffEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Airfield::AirfieldCollectionFactory;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;

namespace UKControllerPlugin {
    namespace Bootstrap {

        void CollectionBootstrap::BootstrapPlugin(
            PersistenceContainer & persistence,
            DependencyLoaderInterface & dependency
        ) {
            // Reset resources
            persistence.airfields = std::move(AirfieldCollectionFactory::Create(dependency));
            persistence.flightplans.reset(new StoredFlightplanCollection);
            persistence.controllerHandoffHandlers.reset(new HandoffEventHandlerCollection);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
