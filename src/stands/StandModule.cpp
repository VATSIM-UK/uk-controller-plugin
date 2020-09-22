#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "StandModule.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Stands {

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
