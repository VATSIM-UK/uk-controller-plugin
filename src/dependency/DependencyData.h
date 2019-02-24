#pragma once

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Represents information about a dependency that the plugin uses
            to build functionality such as controller positions.
        */
        typedef struct DependencyData
        {
            // The path to the dependency on the local filesystem
            const std::string localPath;

            // The path to the dependency on the web API.
            const std::string remotePath;

            // The default value that should be used if the dependency is not available
            const nlohmann::json defaultValue;

            bool operator==(const DependencyData & compare) const {
                return this->localPath == compare.localPath &&
                    this->remotePath == compare.remotePath &&
                    this->defaultValue == compare.defaultValue;
            };
        } DependencyData;

    }  // namespace Dependency
}  // namespace UKControllerPlugin
