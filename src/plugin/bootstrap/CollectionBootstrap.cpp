#include "CollectionBootstrap.h"
#include "PersistenceContainer.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "memory"
#include "metar/MetarEventHandlerCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollectionFactory;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;

namespace UKControllerPlugin::Bootstrap {

    void CollectionBootstrap::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        // Reset resources
        persistence.airfields = AirfieldCollectionFactory::Create(dependency);
        persistence.flightplans = std::make_unique<StoredFlightplanCollection>();
    }
} // namespace UKControllerPlugin::Bootstrap
