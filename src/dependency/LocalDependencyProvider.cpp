#include "pch/stdafx.h"
#include "dependency/LocalDependencyProvider.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Dependency::DependencyData;

namespace UKControllerPlugin {
    namespace Dependency {

        LocalDependencyProvider::LocalDependencyProvider(WinApiInterface & windowsApi)
            : windowsApi(windowsApi)
        {

        }

        /*
            Return a dependency from the local filesystem
        */
        nlohmann::json LocalDependencyProvider::GetDependency(const DependencyData dependency) {
            if (!windowsApi.FileExists(dependency.localPath)) {
                LogWarning("Local dependency file does not exist, loading default " + dependency.localPath);
                return dependency.defaultValue;
            }

            try {
                return nlohmann::json::parse(windowsApi.ReadFromFile(dependency.localPath));
            } catch (nlohmann::json::exception) {
                LogInfo("Local dependency invalid, loading default " + dependency.localPath);
                return dependency.defaultValue;
            }
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
