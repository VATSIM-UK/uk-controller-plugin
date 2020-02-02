#pragma once
#include "dependency/DependencyLoaderInterface.h"
#include "api/ApiInterface.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            A class that is responsible for loading dependencies
            off of the filesystem.
        */
        class DependencyLoader : public DependencyLoaderInterface
        {
            public:
                DependencyLoader(
                    UKControllerPlugin::Windows::WinApiInterface& filesystem
                );

                nlohmann::json LoadDependency(std::string key , nlohmann::json defaultValue) noexcept override;

            private:

                void LoadDependencyMap(void);
                bool ValidDependency(const nlohmann::json& dependency) const;

                // The filesystem.
                UKControllerPlugin::Windows::WinApiInterface& filesystem;


                // A map of dependency to files.
                std::map<std::string, std::string> fileMap;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
