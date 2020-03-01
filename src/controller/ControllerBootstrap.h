#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Controller {

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
        );
    }  // namespace Controller
}  // namespace UKControllerPlugin
