#pragma once
#include "dependency/DependencyLoaderInterface.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Handoff {

        extern const std::string handoffOrdersDependencyKey;
        extern const std::string handoffSidMappingsDependency;

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
        );
    }  // namespace Handoff
}  // namespace UKControllerPlugin
