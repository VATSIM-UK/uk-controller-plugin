#include "pch/stdafx.h"
#include "DependencyLoader.h"


namespace UKControllerPlugin {
    namespace Dependency {

        DependencyLoader::DependencyLoader(UKControllerPlugin::Windows::WinApiInterface& filesystem)
            : filesystem(filesystem)
        {
            this->LoadDependencyMap();
        }

        /*
            Loads the requested dependency, but returns the default on error.
        */
        nlohmann::json DependencyLoader::LoadDependency(std::string key, nlohmann::json defaultValue) noexcept
        {
            if (!this->fileMap.count(key)) {
                LogWarning("Dependency " + key + " does not exist in file map");
                return defaultValue;
            }

            if (!this->filesystem.FileExists("dependencies/" + this->fileMap[key])) {
                LogWarning("Dependency " + key + " does not exist on filesystem");
                return defaultValue;
            }

            try
            {
                return nlohmann::json::parse(this->filesystem.ReadFromFile("dependencies/" + this->fileMap[key]));
            } catch (nlohmann::json::exception) {
                LogWarning("Unable to load dependency " + key + ", it is not valid JSON");
            }

            return defaultValue;
        }

        /*
            Creates the dependency filemap based on the dependency list.
        */
        void DependencyLoader::LoadDependencyMap(void)
        {
            if (!this->filesystem.FileExists("dependencies/dependency-list.json")) {
                LogWarning("No dependency list downloaded, dependencies not loaded");
                return;
            }

            nlohmann::json dependencies;
            try
            {
                dependencies = nlohmann::json::parse(this->filesystem.ReadFromFile("dependencies/dependency-list.json"));
            } catch (nlohmann::json::exception) {
                LogWarning("Unable to parse JSON in dependency list, dependencies not loaded");
                return;
            }

            for (nlohmann::json::const_iterator it = dependencies.cbegin(); it != dependencies.cend(); ++it)
            {
                if (!this->ValidDependency(*it)) {
                    LogWarning("Invalid dependency data, skipping load: " + it->dump());
                    continue;
                }

                this->fileMap[it->at("key")] = it->at("local_file");
            }

            LogInfo("Loaded local dependency file map");
        }

        bool DependencyLoader::ValidDependency(const nlohmann::json& dependency) const
        {
            return dependency.is_object() &&
                dependency.contains("key") &&
                dependency.at("key").is_string() &&
                dependency.contains("local_file") &&
                dependency.at("local_file").is_string();
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
