#pragma once
#include "dependency/DependencyProviderInterface.h"

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Provides access to dependency files via the UKCP API
        */
        class LocalDependencyProvider : public UKControllerPlugin::Dependency::DependencyProviderInterface
        {
            public:
                LocalDependencyProvider(
                    UKControllerPlugin::Windows::WinApiInterface & windowsApi
                );

                nlohmann::json GetDependency(
                    const UKControllerPlugin::Dependency::DependencyData dependency
                ) const override;

                std::string GetProviderType(void) const override;

            private:
                
                // For retrieving files on the filesystem
                UKControllerPlugin::Windows::WinApiInterface & windowsApi;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
