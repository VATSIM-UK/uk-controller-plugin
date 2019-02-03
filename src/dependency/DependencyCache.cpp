#include "pch/stdafx.h"
#include "dependency/DependencyCache.h"

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            Adds a dependency to the cache, throws an exception if already exists.
        */
        void DependencyCache::AddDependency(const std::string dependency, const std::string data)
        {
            if (this->dependencyMap.count(dependency) > 0) {
                throw std::invalid_argument("Dependency " + dependency + " already registered.");
            }

            dependencyMap.insert(std::make_pair(dependency, data));
        }

        /*
            Add a JSON dependency
        */
        void DependencyCache::AddJsonDependency(std::string dependency, const nlohmann::json data)
        {
            if (this->jsonDependencies.count(dependency) > 0) {
                LogWarning("JSON dependency already registered: " + dependency);
                return;
            }

            jsonDependencies.insert(std::make_pair(dependency, data));
        }

        /*
            Returns the number of items in the cache.
        */
        size_t DependencyCache::DependencyCount(void) const
        {
            return this->dependencyMap.size();
        }

        size_t DependencyCache::JsonDependencyCount(void) const
        {
            return this->jsonDependencies.size();
        }

        /*
            Gets a dependency from the cache, throws an exception if it doesn't exists.
        */
        const std::string DependencyCache::GetDependency(const std::string dependency) const
        {
            if (this->dependencyMap.count(dependency) == 0) {
                throw std::out_of_range("Dependency " + dependency + " not found.");
            }

            return this->dependencyMap.at(dependency);
        }

        /*
            Returns a JSON dependency
        */
        nlohmann::json DependencyCache::GetJsonDependency(const std::string dependency) const
        {
            if (this->jsonDependencies.count(dependency) == 0) {
                LogWarning("Tried to access invalid dependency " + dependency);
                return nlohmann::json();
            }

            return this->jsonDependencies.at(dependency);
        }

        /*
            Returns true if the dependency exists.
        */
        bool DependencyCache::HasDependency(std::string dependency) const
        {
            return this->dependencyMap.count(dependency) > 0;
        }

        bool DependencyCache::HasJsonDependency(std::string dependency) const
        {
            return this->jsonDependencies.find(dependency) != this->jsonDependencies.cend();
        }
    }  // namespace Dependency
}  // namespace UKControllerPlugin
