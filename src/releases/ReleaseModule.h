#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Releases {

        extern const std::string enrouteReleaseTypesDependency;

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );
    }  // namespace Releases
}  // namespace UKControllerPlugin
