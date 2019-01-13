#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

/*

*/
namespace UKControllerPlugin {
    namespace Log {

        /*
            A class for bootstrapping the logger.
        */
        class LoggerBootstrap
        {
            public:
                static void Bootstrap(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence,
                    bool nullLogger
                );
        };
    }  // namespace Log
}  // namespace UKControllerPlugin
