#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Stands {

        extern const std::string standDependency;

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );
    }  // namespace Stands
}  // namespace UKControllerPlugin
