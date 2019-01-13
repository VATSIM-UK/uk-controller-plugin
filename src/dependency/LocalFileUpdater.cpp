#include "pch/stdafx.h"
#include "dependency/LocalFileUpdater.h"
#include "windows/WinApiInterface.h"
#include "api/RemoteFileManifest.h"
#include "helper/HelperFunctions.h"
#include "api/ApiInterface.h"
#include "api/ApiException.h"
#include "api/ApiNotFoundException.h"

using UKControllerPlugin::Api::RemoteFileManifest;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Api::RemoteFile;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiNotFoundException;

namespace UKControllerPlugin {
    namespace Dependency {

        const std::string LocalFileUpdater::DEPENDENCY_FOLDER = "dependencies";

        const std::string LocalFileUpdater::HASH_FILE = "dependencies/filehash.json";

        LocalFileUpdater::LocalFileUpdater(
            WinApiInterface & winApi,
            const ApiInterface & webApi
        ) : winApi(winApi), webApi(webApi)
        {
        }


        void LocalFileUpdater::UpdateLocalFilesFromManifest(
            const RemoteFileManifest & manifest
        ) {

            // If we can't create the dependency folder, stop.
            if (!this->winApi.CreateFolder(LocalFileUpdater::DEPENDENCY_FOLDER)) {
                LogCritical("Could not create the dependencies folder");
                return;
            }

            nlohmann::json hashCache = this->LoadFileHashCache();
            std::map<std::string, std::string> newHashCache;

            // Download new or outdated files, whilst keeping the hash cache up to date.
            for (RemoteFileManifest::const_iterator it = manifest.cbegin(); it != manifest.cend(); ++it) {

                // If the file doesn't exist, or is outdated
                if (!this->winApi.FileExists(LocalFileUpdater::DEPENDENCY_FOLDER + "/" + it->filename) ||
                    !this->FileMatchesRemote(hashCache, *it)
                ) {
                    // We only want to add the new file to the md5 cache if it successfully downloads.
                    LogInfo("Downloading dependency file " + it->filename);
                    if (this->FetchFileFromRemoteServer(it->uri, it->filename)) {
                        newHashCache[it->filename] = it->hash;
                    }
                } else {
                    newHashCache[it->filename] = it->hash;
                }
            }

            // Write the hash cache to disk.
            try {
                this->winApi.WriteToFile(LocalFileUpdater::HASH_FILE, nlohmann::json(newHashCache).dump(4), true);
            } catch (...) {
                LogError("Unable to write hash cache for local files");
            }
        }

        /*
            Downloads a remote file from the server.
        */
        bool LocalFileUpdater::FetchFileFromRemoteServer(std::string uri, std::string filename)
        {
            // Write the file to disk and return false if it fails.
            try {
                std::string fileData = this->webApi.FetchRemoteFile(uri);
                this->winApi.WriteToFile(LocalFileUpdater::DEPENDENCY_FOLDER + "/" + filename, fileData, true);
            } catch (ApiNotFoundException notFound) {
                LogError("File download returned not found: " + filename + "(" + uri + ")");
                return false;
            } catch (ApiException apiException) {
                LogError("File download returned error: " + filename + "(" + uri + ")");
                return false;
            } catch (...) {
                LogError("Unknown error when downloading file: " + filename + "(" + uri + ")");
                return false;
            }

            return true;
        }

        /*
            Returns true if the local version of the file matches the remote by comparing
            md5 hashes.
        */
        bool LocalFileUpdater::FileMatchesRemote(nlohmann::json hashCache, const RemoteFile & remote)
        {
            return hashCache[remote.filename].is_string() && remote.hash == hashCache[remote.filename];
        }

        /*
            Loads the data from the hash cache file.
        */
        nlohmann::json LocalFileUpdater::LoadFileHashCache(void)
        {
            try {
                return (this->winApi.FileExists(LocalFileUpdater::HASH_FILE))
                    ? nlohmann::json::parse(this->winApi.ReadFromFile(LocalFileUpdater::HASH_FILE))
                    : "{}"_json;
            }
            catch (...) {
                // Either going to be an nlohmann::json::exception or ifstream::failure
                return "{}"_json;
            }
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
