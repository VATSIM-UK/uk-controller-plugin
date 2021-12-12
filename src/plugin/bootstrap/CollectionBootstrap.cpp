#include "CollectionBootstrap.h"
#include "PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "metar/MetarEventHandlerCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;

namespace UKControllerPlugin::Bootstrap {

    void CollectionBootstrap::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        // Reset resources
        persistence.flightplans = std::make_unique<StoredFlightplanCollection>();
    }
} // namespace UKControllerPlugin::Bootstrap
