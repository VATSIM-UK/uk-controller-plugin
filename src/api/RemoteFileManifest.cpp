#include "pch/stdafx.h"
#include "api/RemoteFileManifest.h"

using UKControllerPlugin::Api::RemoteFile;

namespace UKControllerPlugin {
    namespace Api {

        /*
            Add a file to the manifest.
        */
        void RemoteFileManifest::AddFile(RemoteFile file)
        {
            if (!this->files.insert(file).second) {
                throw std::invalid_argument("The remote file " + file.filename + " is already in the manifest");
            }
        }

        /*
            Returns whether or not the manifest is empty.
        */
        bool RemoteFileManifest::IsEmpty(void) const
        {
            return this->files.size() == 0;
        }

        /*
            Converts the manifest to JSON format.
        */
        std::string RemoteFileManifest::ToJsonString(void) const
        {
            nlohmann::json fileJson;
            fileJson["manifest"] = {};

            for (std::set<RemoteFile>::const_iterator it = this->files.cbegin(); it != this->files.cend(); ++it) {
                fileJson["manifest"][it->filename] = {
                    {"uri", it->uri},
                    {"md5", it->hash}
                };
            }

            return fileJson.dump(4);
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
