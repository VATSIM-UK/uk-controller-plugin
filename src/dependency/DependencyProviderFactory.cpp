#include "pch/stdafx.h"
#include "dependency/DependencyProviderFactory.h"
#include "dependency/DependencyProviderInterface.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "dependency/ApiDependencyProvider.h"
#include "dependency/LocalDependencyProvider.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::ApiDependencyProvider;
using UKControllerPlugin::Dependency::LocalDependencyProvider;

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Return a provider of dependencies. If not web API authorised, return the
            local provider. Otherwise use the web one.
        */
        std::unique_ptr<DependencyProviderInterface> GetDependencyProvider(
            ApiInterface & webApi,
            WinApiInterface & windowsApi,
            bool webApiAuthorised
        ) {
            if (!webApiAuthorised) {
                return std::make_unique<LocalDependencyProvider>(windowsApi);
            }

            return std::make_unique<ApiDependencyProvider>(webApi, windowsApi);
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
