#pragma once
#include "dependency/DependencyProviderInterface.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Provides access to dependency files via the UKCP API
        */
        class ApiDependencyProvider : public UKControllerPlugin::Dependency::DependencyProviderInterface
        {
            public:
                ApiDependencyProvider(
                    const UKControllerPlugin::Api::ApiInterface & webApi,
                    UKControllerPlugin::Windows::WinApiInterface & windowsApi
                );

                // Inherited via DependencyProviderInterface
                nlohmann::json GetDependency(
                    const UKControllerPlugin::Dependency::DependencyData dependency
                ) const override;

                std::string GetProviderType(void) const override;

            private:

                // Provides interactions with the web api
                const UKControllerPlugin::Api::ApiInterface & webApi;

                // For saving files on the filesystem
                UKControllerPlugin::Windows::WinApiInterface & windowsApi;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
