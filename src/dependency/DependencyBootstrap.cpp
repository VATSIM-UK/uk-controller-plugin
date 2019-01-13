#include "pch/stdafx.h"
#include "dependency/DependencyBootstrap.h"
#include "api/ApiInterface.h"
#include "api/RemoteFileManifest.h"
#include "api/RemoteFileManifestFactory.h"
#include "dependency/LocalFileUpdater.h"
#include "dependency/DependencyCacheFactory.h"
#include "api/ApiException.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFileManifestFactory;
using UKControllerPlugin::Dependency::LocalFileUpdater;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Dependency::DependencyCacheFactory;
using UKControllerPlugin::Api::ApiException;

namespace UKControllerPlugin {
    namespace Bootstrap {

        const std::string DependencyBootstrap::manifestFile = "dependencies/manifest.json";

        /*
            Bootstrap the dependency cache. Get all the dependencies from the server and create the cache.
        */
        UKControllerPlugin::Dependency::DependencyCache DependencyBootstrap::Bootstrap(
            ApiInterface & api,
            WinApiInterface & windows,
            CurlInterface & curl
        ) {
            RemoteFileManifest manifest;
            try {
                manifest = api.FetchDependencyManifest();
                windows.WriteToFile(DependencyBootstrap::manifestFile, manifest.ToJsonString(), true);
                LocalFileUpdater updater(windows, api);
                updater.UpdateLocalFilesFromManifest(manifest);
            } catch (ApiException e) {
                // Nothing
            }

            // If the dependency manifest is empty, that means either the API threw an exception
            // or the manifest was corrupt. In this case, fallback to local file.
            if (manifest.IsEmpty()) {
                LogInfo("Unable to fetch remote manifest, falling back to local");
                RemoteFileManifestFactory factory(windows);
                manifest = factory.CreateFromLocalFile(DependencyBootstrap::manifestFile);
            }

            return *DependencyCacheFactory::Create("dependencies", manifest, windows);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
