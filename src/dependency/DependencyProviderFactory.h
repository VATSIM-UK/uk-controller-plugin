#pragma once
#include "dependency/DependencyProviderInterface.h"

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Api {
        class ApiInterface;
    }  // namespace Api
    namespace Dependency {
        class DependencyProviderInterface;
    }  // namespace Dependency
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Dependency {

        std::unique_ptr<UKControllerPlugin::Dependency::DependencyProviderInterface> GetDependencyProvider(
            UKControllerPlugin::Api::ApiInterface & webApi,
            UKControllerPlugin::Windows::WinApiInterface & windowsApi,
            bool webApiAuthorised
        );
    }  // namespace Dependency
}  // namespace UKControllerPlugin
