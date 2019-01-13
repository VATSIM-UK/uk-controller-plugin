#include "pch/stdafx.h"
#include "dependency/DependencyCacheFactory.h"
#include "dependency/DependencyCache.h"
#include "windows/WinApi.h"
#include "api/RemoteFileManifest.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Creates the dependency cache from files on the local filesystem and the remote server.
            If the remote file is different, downloads it and saves it locally.
        */
        std::unique_ptr<DependencyCache> DependencyCacheFactory::Create(
            std::string dependencyFolder,
            const UKControllerPlugin::Api::RemoteFileManifest & manifest,
            UKControllerPlugin::Windows::WinApiInterface & winApi
        ) {
            std::unique_ptr<DependencyCache> cache(new DependencyCache);

            // Loop the files in the manifest.
            for (RemoteFileManifest::const_iterator it = manifest.cbegin(); it != manifest.cend(); ++it) {
                try {
                    if (
                        winApi.FileExists(dependencyFolder + "/" + it->filename) && !cache->HasDependency(it->filename)
                    ) {
                        cache->AddDependency(it->filename, winApi.ReadFromFile(dependencyFolder + "/" + it->filename));
                    }
                } catch (std::ifstream::failure) {

                }

            }

            return cache;
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
