#include "FlightRuleCollection.h"
#include "FlightRuleCollectionFactory.h"
#include "FlightRuleModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::FlightRules {

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencyLoader)
    {
        container.flightRules =
            CreateCollection(dependencyLoader.LoadDependency("DEPENDENCY_FLIGHT_RULES", nlohmann::json::array()));
    }
} // namespace UKControllerPlugin::FlightRules
