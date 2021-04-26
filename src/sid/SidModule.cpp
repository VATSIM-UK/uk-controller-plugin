#include "pch/stdafx.h"
#include "sid/SidModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "sid/SidCollectionFactory.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Sid {

        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container,
            Dependency::DependencyLoaderInterface& dependencyProvider
        )
        {
            nlohmann::json sidData = dependencyProvider.LoadDependency("DEPENDENCY_SIDS", nlohmann::json::array());
            container.sids = MakeSidCollection(sidData);
        }
    }  // namespace Sid
}  // namespace UKControllerPlugin
