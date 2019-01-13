#pragma once
#include <set>
#include "api/RemoteFile.h"

namespace UKControllerPlugin {
    namespace Api {

        /*
            A class that represents a manifest of remote files, that
            we may wish to download.
        */
        class RemoteFileManifest
        {
            public:
                void AddFile(UKControllerPlugin::Api::RemoteFile file);
                bool IsEmpty(void) const;
                std::string ToJsonString(void) const;

                typedef std::set<UKControllerPlugin::Api::RemoteFile> ManifestSet;
                typedef ManifestSet::iterator iterator;
                typedef ManifestSet::const_iterator const_iterator;
                iterator begin(void) { return files.begin(); }
                iterator end(void) { return files.end(); }
                const_iterator cbegin() const { return files.cbegin(); }
                const_iterator cend() const { return files.cend(); }

            private:

                // All the files in the manifest.
                std::set<RemoteFile> files;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
