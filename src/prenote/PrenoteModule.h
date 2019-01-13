#pragma once

namespace UKControllerPlugin {
    namespace Dependency {
        class DependencyCache;
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
                    const UKControllerPlugin::Dependency::DependencyCache & dependency
                );

                // The file to read to get the prenote data
                static const std::string dependencyFile;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
