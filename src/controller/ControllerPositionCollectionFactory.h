#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    }  // namespace Dependency
    namespace Controller {
        class ControllerPositionCollection;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Controller {

        class ControllerPositionCollectionFactory
        {
            public:
                static std::unique_ptr<ControllerPositionCollection> Create(
                    UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
                );
                static const std::string requiredDependency;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
