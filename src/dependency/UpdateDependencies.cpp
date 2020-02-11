#include "pch/stdafx.h"
#include "dependency/UpdateDependencies.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Dependency {

        void UpdateDependencies(const ApiInterface& api, WinApiInterface& filesystem)
        {
            // Download the dependency list and save it to the filesystem
            LogInfo("Downloading dependency data");
            nlohmann::json dependencyList;
            try {
                dependencyList = api.GetDependencyList();
                filesystem.WriteToFile("dependencies/dependency-list.json", dependencyList.dump(), true);
                LogInfo("Downloaded dependency list");
            } catch (ApiException exception) {
                LogError("Unable to download dependency list: " + std::string(exception.what()));
                return;
            }

            // Download all the dependencies themselves and save them to the filesystem
            for (
                nlohmann::json::const_iterator it = dependencyList.cbegin();
                it != dependencyList.cend();
                ++it
            ) {
                if (!ValidDependency(*it)) {
                    LogWarning("Invalid dependency in dependency list, skipping download: " + it->dump());
                    continue;
                }

                try {
                    std::string filename = it->at("uri");
                    filesystem.WriteToFile(
                        "dependencies/" + it->at("local_file").get<std::string>(),
                        api.GetUri(it->at("uri").get<std::string>()).dump(),
                        true
                    );
                } catch (ApiException exception) {
                    LogError("Unable to download dependency file: " + std::string(exception.what()));
                    continue;
                }
            }

            LogInfo("Finished downloading dependency files");
        }

        bool ValidDependency(const nlohmann::json& dependency)
        {
            return dependency.is_object() &&
                dependency.contains("key") &&
                dependency.at("key").is_string() &&
                dependency.contains("uri") &&
                dependency.at("uri").is_string() &&
                dependency.contains("local_file") &&
                dependency.at("local_file").is_string();
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
