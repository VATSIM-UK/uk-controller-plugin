#pragma once

namespace UKControllerPlugin {
    namespace Api {

        /*
            Represents a file stored on some remote server.
        */
        typedef struct RemoteFile
        {
            std::string filename;
            std::string uri;
            std::string hash;
            bool operator==(const RemoteFile& compare) const
            {
                return filename == compare.filename;
            };
            bool operator<(const RemoteFile& compare) const
            {
                return filename < compare.filename;
            };
            bool operator>(const RemoteFile& compare) const
            {
                return filename > compare.filename;
            };
        } RemoteFile;

    }  // namespace Api
}  // namespace UKControllerPlugin
