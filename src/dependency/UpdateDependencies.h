#pragma once
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Dependency {

        std::map<std::string, nlohmann::json> LoadDependencyList(
            const nlohmann::json dependencyList
        );
        std::map<std::string, nlohmann::json> LoadDependencyListFromFilesystem(
            UKControllerPlugin::Windows::WinApiInterface& filesystem
        );
        bool NeedsDownload(
            const std::map<std::string, nlohmann::json>& local,
            const std::map<std::string, nlohmann::json>& remote,
            std::string key
        );
        void UpdateDependencies(
            const UKControllerPlugin::Api::ApiInterface & api,
            UKControllerPlugin::Windows::WinApiInterface & filesystem
        );

        bool ValidDependency(const nlohmann::json & dependency);
    }  // namespace Dependency
}  // namespace UKControllerPlugin
