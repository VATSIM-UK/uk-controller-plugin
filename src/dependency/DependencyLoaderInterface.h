#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Interface for classes that load dependencies
        */
        class DependencyLoaderInterface
        {
            public:

                virtual nlohmann::json LoadDependency(
                    std::string key,
                    nlohmann::json defaultValue
                ) noexcept = 0;
        };

    }  // namespace Dependency
}  // namespace UKControllerPlugin
