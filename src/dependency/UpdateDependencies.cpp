#include "pch/stdafx.h"
#include "dependency/UpdateDependencies.h"
#include "api/ApiException.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Dependency {

        const std::wstring dependencyListFile = L"dependencies/dependency-list.json";

        std::map<std::string, nlohmann::json> LoadDependencyList(const nlohmann::json dependencyList)
        {
            std::map<std::string, nlohmann::json> dependencies;
            for (
                nlohmann::json::const_iterator it = dependencyList.cbegin();
                it != dependencyList.cend();
                ++it
             ) {
                if (!ValidDependency(*it)) {
                    LogWarning("Invalid dependency in dependency list, skipping: " + it->dump());
                    continue;
                }

                dependencies[it->at("key").get<std::string>()] = *it;
            }

            return dependencies;
        }

        /*
            Load up a map of key -> dependency
        */
        std::map<std::string, nlohmann::json> LoadDependencyListFromFilesystem(WinApiInterface& filesystem)
        {
            if (!filesystem.FileExists(dependencyListFile)) {
                LogInfo("Local dependencies file not found");
                return {};
            }

            nlohmann::json dependencyList;
            try {
                return LoadDependencyList(nlohmann::json::parse(filesystem.ReadFromFile(dependencyListFile)));
            }
            catch (std::exception e)
            {
                LogError("Exception thrown when reading dependency list file: " + std::string(e.what()));
            }

            return {};
        }

        /*
            If the local dependency is older than the newer one, download it.
        */
        bool NeedsDownload(
            const std::map<std::string, nlohmann::json>& local,
            const std::map<std::string, nlohmann::json>& remote,
            std::string key
        ) {
            return !local.count(key) ||
                local.at(key).at("updated_at").get<int>() < remote.at(key).at("updated_at").get<int>();
        }

        void UpdateDependencies(const ApiInterface& api, WinApiInterface& filesystem)
        {
            // Download the dependency list and save it to the filesystem
            LogInfo("Loading existing dependencies");
            std::map<std::string, nlohmann::json> existingDependencies = LoadDependencyListFromFilesystem(filesystem);

            LogInfo("Downloading new dependency list");
            std::map<std::string, nlohmann::json> newDependencies;
            try {
                newDependencies = LoadDependencyList(api.GetDependencyList());
                LogInfo("Downloaded new dependency list");
            } catch (ApiException exception) {
                LogError("Unable to download dependency list: " + std::string(exception.what()));
                return;
            }

            // Loop the dependencies and download just the ones we need.
            nlohmann::json dependencyListToSave = nlohmann::json::array();
            for (
                std::map<std::string, nlohmann::json>::const_iterator it = newDependencies.cbegin();
                it != newDependencies.cend();
                ++it
            ) {
                std::wstring widePath = HelperFunctions::ConvertToWideString(it->second.at("local_file"));

                if (
                    filesystem.FileExists(L"dependencies/" + widePath) &&
                    !NeedsDownload(existingDependencies, newDependencies, it->first)
                ) {
                    LogInfo("Dependency " + it->first + " is up to date, skipping download");
                    dependencyListToSave.push_back(it->second);
                    continue;
                }

                try {
                    LogInfo("Dependency " + it->first + " has a new version available, downloading");
                    filesystem.WriteToFile(
                        L"dependencies/" + widePath,
                        api.GetUri(it->second.at("uri").get<std::string>()).dump(),
                        true
                    );
                    dependencyListToSave.push_back(it->second);
                    LogInfo("New version of dependency " + it->first + " has been downloaded");
                }
                catch (ApiException exception) {
                    LogError("Unable to download dependency file: " + std::string(exception.what()));
                    continue;
                }
            }

            filesystem.WriteToFile(dependencyListFile, dependencyListToSave.dump(), true);
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
                dependency.at("local_file").is_string() &&
                dependency.contains("updated_at") &&
                dependency.at("updated_at").is_number_integer();
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
