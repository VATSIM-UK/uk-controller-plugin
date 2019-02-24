#pragma once
#include "dependency/DependencyData.h"

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Provides access to dependency files as required by the plugin.
        */
        class DependencyProviderInterface
        {
            public:

                /*
                    Returns the JSON for a given dependency
                */
                virtual nlohmann::json GetDependency(
                    UKControllerPlugin::Dependency::DependencyData dependency
                ) = 0;
        };

    }  // namespace Dependency
}  // namespace UKControllerPlugin
