#include "pch/stdafx.h"
#include "dependency/ApiDependencyProvider.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyData;

namespace UKControllerPlugin {
    namespace Dependency {

        ApiDependencyProvider::ApiDependencyProvider(const ApiInterface & webApi, WinApiInterface & windowsApi)
            : webApi(webApi), windowsApi(windowsApi)
        {

        }

        /*
            Get the dependency from the API and save it to the local filesystem. Use the local filesystem
            if the download fails.
        */
        nlohmann::json ApiDependencyProvider::GetDependency(const DependencyData dependency) const
        {
            if (!windowsApi.FileExists(dependency.localPath)) {
                LogInfo("Local dependency does not exist, creating file " + dependency.localPath);
                windowsApi.WriteToFile(dependency.localPath, dependency.defaultValue.dump(), true);
            }

            nlohmann::json dependencyData;

            try {
                dependencyData = webApi.GetDependency(dependency);
                windowsApi.WriteToFile(dependency.localPath, dependencyData.dump(), true);
            }
            catch (ApiException exception) {
                LogError(
                    "ApiException when downloading dependency data for " + dependency.localPath +
                        ": " + std::string(exception.what())
                );

                try {
                    LogInfo("Trying local dependency file");
                    return nlohmann::json::parse(windowsApi.ReadFromFile(dependency.localPath));
                } catch (nlohmann::json::exception) {
                    LogError("Local data corrupt, returning default");
                    return dependency.defaultValue;
                }
            }

            LogInfo("Updated dependency " + dependency.localPath);
            return dependencyData;
        }

        /*
            Return the provider type
        */
        std::string ApiDependencyProvider::GetProviderType(void) const
        {
            return "ApiDependencyProvider";
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
