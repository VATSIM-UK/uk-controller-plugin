#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyLoaderInterface;
    }  // namespace Dependency
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace Prenote {
        class DeparturePrenote;
        class PrenoteEventHandler;
    }  // namespace Prenote
    namespace Controller {
        class ControllerPositionCollection;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            Bootstraps the prenote module.
        */
        class PrenoteModule
        {
            public:
                static void BootstrapPlugin(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence,
                    UKControllerPlugin::Dependency::DependencyLoaderInterface& dependency
                );

                // The file to read to get the prenote data
                static const std::string dependencyKey;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
