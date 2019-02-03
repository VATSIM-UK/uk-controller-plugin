#include "pch/stdafx.h"
#include "dependency/LoadNewDependencies.h"
#include "dependency/DependencyCache.h"
#include "bootstrap/PersistenceContainer.h"
#include "hold/HoldModule.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Load all the "new" dependencies into the cache.
        */
        void LoadNewDependencies(const PersistenceContainer & container, DependencyCache * const dependencies)
        {
            UKControllerPlugin::Hold::LoadDependencies(dependencies, *container.api, *container.windows);
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
