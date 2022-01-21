namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Runway {
    void BootstrapPlugin(
        Bootstrap::PersistenceContainer& container, Dependency::DependencyLoaderInterface& dependencyLoader);
} // namespace UKControllerPlugin::Runway
