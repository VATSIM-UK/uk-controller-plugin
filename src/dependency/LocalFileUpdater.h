#pragma once

namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
    namespace Api {
        class ApiInterface;
    }  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {
        class RemoteFileManifest;
        struct RemoteFile;
    }  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Class for updating local files, based on a manifest file.
        */
        class LocalFileUpdater
        {
            public:
                LocalFileUpdater(
                    UKControllerPlugin::Windows::WinApiInterface & winApi,
                    const UKControllerPlugin::Api::ApiInterface & webApi
                );

                void UpdateLocalFilesFromManifest(
                    const UKControllerPlugin::Api::RemoteFileManifest & manifest
                );

                // The folder our dependencies are stored in
                static const std::string DEPENDENCY_FOLDER;

                // The name of the file where hashes are cached
                static const std::string HASH_FILE;

            private:
                bool FetchFileFromRemoteServer(std::string uri, std::string filename);
                bool FileMatchesRemote(nlohmann::json hashCache, const UKControllerPlugin::Api::RemoteFile & remote);
                nlohmann::json LoadFileHashCache(void);

                // Wrapper around the Windows API
                UKControllerPlugin::Windows::WinApiInterface & winApi;

                // An interface to cURL
                const UKControllerPlugin::Api::ApiInterface & webApi;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
