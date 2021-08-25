#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Prenote {
        class DeparturePrenote;
        class PrenoteEventHandler;
    } // namespace Prenote
    namespace Controller {
        class ControllerPositionCollection;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {

    /*
        Bootstraps the prenote module.
    */
    class PrenoteModule
    {
        public:
        static void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer& persistence,
            UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency);

        private:
        [[nodiscard]] static auto DependencyKey() -> std::string;
    };
} // namespace UKControllerPlugin::Prenote
