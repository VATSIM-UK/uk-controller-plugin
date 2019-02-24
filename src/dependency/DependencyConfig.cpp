#pragma once
#include "pch/stdafx.h"
#include "dependency/DependencyConfig.h"
#include "dependency/DependencyData.h"

using UKControllerPlugin::Dependency::DependencyData;
using UKControllerPlugin::Dependency::DependencyConfig;

namespace UKControllerPlugin {
    namespace Dependency {

        // Holds
        const DependencyData DependencyConfig::holds = {
            "holds",
            "dependencies/holds.json",
            "[]"_json
        };

        const DependencyData DependencyConfig::genericHoldProfiles = {
            "hold/profile",
            "dependencies/generic-hold-profiles.json",
            "[]"_json
        };

        const DependencyData DependencyConfig::userHoldProfiles = {
            "hold/profile/user",
            "dependencies/user-hold-profiles.json",
            "[]"_json
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
