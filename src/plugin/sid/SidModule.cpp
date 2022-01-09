#include "FlightplanSidMapper.h"
#include "SidCollection.h"
#include "SidCollectionFactory.h"
#include "SidModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin::Sid {

    void BootstrapPlugin(
        Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& dependencyProvider)
    {
        nlohmann::json sidData = dependencyProvider.LoadDependency("DEPENDENCY_SIDS", nlohmann::json::array());
        container.sids = MakeSidCollection(sidData);
        container.sidMapper =
            std::make_unique<FlightplanSidMapper>(*container.airfields, *container.runwayCollection, *container.sids);
    }
} // namespace UKControllerPlugin::Sid
