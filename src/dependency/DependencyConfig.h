#pragma once
#include "dependency/DependencyData.h"

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Config for all the dependencies that the plugin has
        */
        typedef struct DependencyConfig {

            // Holds
            static const DependencyData holds;
            static const DependencyData genericHoldProfiles;
            static const DependencyData userHoldProfiles;

        } DependencyConfig;
    }  // namespace Dependency
}  // namespace UKControllerPlugin
