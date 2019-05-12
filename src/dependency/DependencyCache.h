#pragma once

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Class that caches dependencies that have been loaded by the dependency updater.
        */
        class DependencyCache
        {
            public:
                void AddDependency(const std::string dependency, const std::string data);
                void AddJsonDependency(std::string dependency, const nlohmann::json dependencyData);
                size_t DependencyCount(void) const;
                size_t JsonDependencyCount(void) const;
                virtual const std::string GetDependency(const std::string dependency) const;
                nlohmann::json GetJsonDependency(const std::string dependency) const;
                bool HasDependency(std::string dependency) const;
                bool HasJsonDependency(std::string dependency) const;

            private:
                std::map<const std::string, const std::string> dependencyMap;

                std::map<const std::string, const nlohmann::json> jsonDependencies;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
