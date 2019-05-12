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
                ) const = 0;

                /*
                    Returns the type of provider
                */
                virtual std::string GetProviderType(void) const = 0;
        };

    }  // namespace Dependency
}  // namespace UKControllerPlugin
