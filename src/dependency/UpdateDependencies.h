#pragma once
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Dependency {
        void UpdateDependencies(
            const UKControllerPlugin::Api::ApiInterface & api,
            UKControllerPlugin::Windows::WinApiInterface & filesystem
        );

        bool ValidDependency(const nlohmann::json & dependency);
    }  // namespace Dependency
}  // namespace UKControllerPlugin
