#pragma once
#include "pch/stdafx.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "StandModule.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Stands {

        // The tag item id for assigned stand
        const int assignedStandTagItemId = 110;

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
