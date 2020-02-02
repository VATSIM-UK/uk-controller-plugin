#pragma once
#include "dependency/DependencyLoaderInterface.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Handoff {

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
        );
    }  // namespace Handoff
}  // namespace UKControllerPlugin
