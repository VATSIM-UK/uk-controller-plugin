#include "RunwayCollection.h"
#include "RunwayCollectionFactory.h"
#include "RunwayModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::Runway {

    void
    BootstrapPlugin(Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& dependencyLoader)
    {
        container.runwayCollection = BuildRunwayCollection(
            dependencyLoader.LoadDependency("DEPENDENCY_RUNWAYS", nlohmann::json::array()), *container.airfields);
    }
} // namespace UKControllerPlugin::Runway
