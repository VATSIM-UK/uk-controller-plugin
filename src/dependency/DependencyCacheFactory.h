#pragma once

// Forward declaratons
namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency

    namespace Api {
        class RemoteFileManifest;
    }  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows

    namespace Curl {
        class CurlInterface;
    }  // namespace Curl
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Builds a dependency cache of file data, from both the remote
            server and the local directories.
        */
        class DependencyCacheFactory
        {
            public:
                static std::unique_ptr<UKControllerPlugin::Dependency::DependencyCache> Create(
                    std::string dependencyFolder,
                    const UKControllerPlugin::Api::RemoteFileManifest & manifest,
                    UKControllerPlugin::Windows::WinApiInterface & winApi
                );
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
