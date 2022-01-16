#include "AircraftModule.h"
#include "AircraftTypeCollection.h"
#include "AircraftTypeCollectionFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::Aircraft {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& loader)
    {
        container.aircraftTypes =
            CollectionFromDependency(loader.LoadDependency("DEPENDENCY_AIRCRAFT", nlohmann::json::array()));
    }
} // namespace UKControllerPlugin::Aircraft
