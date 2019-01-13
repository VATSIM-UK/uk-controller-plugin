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
                size_t DependencyCount(void) const;
                virtual const std::string GetDependency(const std::string dependency) const;
                bool HasDependency(std::string dependency) const;

            private:
                std::map<const std::string, const std::string> dependencyMap;
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
