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
    namespace Bootstrap {

        /*
            A class for bootstrapping externals, such as the windows API.
        */
        class ExternalsBootstrap
        {
            public:
                static void Bootstrap(
                    UKControllerPlugin::Bootstrap::PersistenceContainer & persistence, HINSTANCE instance
                );
        };
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
