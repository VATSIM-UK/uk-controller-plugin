#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Controller {
        class ControllerPositionCollection;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Controller {

    class ControllerPositionCollectionFactory
    {
        public:
        static std::unique_ptr<ControllerPositionCollection> Create(Dependency::DependencyLoaderInterface& dependency);
        static bool DependencyDataValid(nlohmann::json& data);
        [[nodiscard]] static auto GetDependency() -> std::string;
    };
} // namespace UKControllerPlugin::Controller
