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

        const DependencyData DependencyConfig::genericHoldProfiles {
            "dependencies/generic-hold-profiles.json",
            "hold/profile",
            "[]"
        };

        const DependencyData DependencyConfig::userHoldProfiles {
            "dependencies/user-hold-profiles.json",
            "hold/profile/user",
            "[]"
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
