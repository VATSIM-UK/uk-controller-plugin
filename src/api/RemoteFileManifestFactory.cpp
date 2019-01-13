#include "pch/stdafx.h"
#include "api/RemoteFileManifestFactory.h"
#include "api/RemoteFile.h"
#include "api/ApiResponseValidator.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPlugin::Api::ApiResponseValidator;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Api {

        RemoteFileManifestFactory::RemoteFileManifestFactory(
            UKControllerPlugin::Windows::WinApiInterface & winApi
        ) : winApi(winApi)
        {

        }

        /*
            Creates the manifest based on the API response.
        */
        RemoteFileManifest RemoteFileManifestFactory::CreateFromData(nlohmann::json webData) const
        {
            RemoteFileManifest manifest = this->CreateFromJsonData(webData);
            return this->CreateFromJsonData(webData);
        }

        /*
            Creates a manifest based on a local copy of the JSON data, usually used
            when the WebAPI isn't available to us.
        */
        RemoteFileManifest RemoteFileManifestFactory::CreateFromLocalFile(std::string filePath) const
        {
            nlohmann::json manifestJson;
            if (this->winApi.FileExists(filePath)) {
                try {
                    manifestJson = nlohmann::json::parse(
                        this->winApi.ReadFromFile(filePath)
                    );
                } catch (nlohmann::json::exception) {
                    LogError("Local dependency manifest is not valid JSON");
                }
            } else {
                LogError("No local manifest file available");
            }

            return RemoteFileManifestFactory::CreateFromJsonData(manifestJson);
        }

        /*
            Given some JSON data, create a file manifest.
        */
        RemoteFileManifest RemoteFileManifestFactory::CreateFromJsonData(nlohmann::json jsonData) const
        {
            RemoteFileManifest manifest;

            if (!jsonData["manifest"].is_object()) {
                LogError("No dependency manifest available");
                return manifest;
            }

            for (nlohmann::json::iterator it = jsonData["manifest"].begin(); it != jsonData["manifest"].end(); ++it) {

                // Check we have the information we need.
                if (!ApiResponseValidator::ValidateSchema(
                        *it, { { "uri", ApiResponseValidator::string }, { "md5", ApiResponseValidator::string } })
                ) {
                    LogError("Remote file " + it.key() + " was malformed in manifest");
                    continue;
                }


                // Create the file
                RemoteFile file;
                file.filename = it.key();
                file.hash = (*it)["md5"].get<std::string>();
                file.uri = (*it)["uri"].get<std::string>();
                manifest.AddFile(file);
            }

            return manifest;
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
