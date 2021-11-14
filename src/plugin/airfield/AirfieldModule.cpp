#include "AirfieldModule.h"
#include "AirfieldCollection.h"
#include "AirfieldCollectionFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::Airfield {

    const std::string DEPENDENCY_KEY = "DEPENDENCY_AIRFIELD"; // NOLINT

    void
    BootstrapPlugin(Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& dependencyLoader)
    {
        container.airfields = CreateAirfieldCollection(
            dependencyLoader.LoadDependency(DEPENDENCY_KEY, nlohmann::json::array()),
            *container.controllerHierarchyFactory);
    }
} // namespace UKControllerPlugin::Airfield
