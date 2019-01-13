#pragma once
#include "api/RemoteFileManifest.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {

        /*
            Factory for building up manifests based on the API response.
        */
        class RemoteFileManifestFactory
        {
            public:
                RemoteFileManifestFactory(
                    UKControllerPlugin::Windows::WinApiInterface & winApi
                );

                UKControllerPlugin::Api::RemoteFileManifest CreateFromData(
                    nlohmann::json webData
                ) const;

                UKControllerPlugin::Api::RemoteFileManifest CreateFromLocalFile(
                    std::string filePath
                ) const;

            private:
                UKControllerPlugin::Api::RemoteFileManifest CreateFromJsonData(
                    nlohmann::json jsonData
                ) const;

                // For reading and writing off the filesystem
                UKControllerPlugin::Windows::WinApiInterface & winApi;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
