#pragma once
#include "pch/stdafx.h"
#include "dependency/DependencyConfig.h"
#include "dependency/DependencyData.h"

using UKControllerPlugin::Dependency::DependencyData;
using UKControllerPlugin::Dependency::DependencyConfig;

namespace UKControllerPlugin {
    namespace Dependency {

        // Holds
        const DependencyData DependencyConfig::holds {
            "dependencies/holds.json",
            "holds",
            "[]"
        };

        const DependencyData DependencyConfig::holdProfiles {
            "dependencies/hold-profiles.json",
            "hold/profile",
            "[]"
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
