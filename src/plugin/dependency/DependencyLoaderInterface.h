#pragma once

namespace UKControllerPlugin::Dependency {

    /*
        Interface for classes that load dependencies
    */
    class DependencyLoaderInterface
    {
        public:
        virtual ~DependencyLoaderInterface() = default;
        [[nodiscard]] virtual nlohmann::json LoadDependency(std::string key, nlohmann::json defaultValue) noexcept = 0;
    };

} // namespace UKControllerPlugin::Dependency
