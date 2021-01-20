#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

namespace UKControllerPlugin {
    namespace Navaids {

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependencies
        );
        bool NavaidValid(const nlohmann::json& navaid);
    }  // namespace Navaids
}  // namespace UKControllerPlugin
