#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            A class for bootstrapping the various event handlers.
        */
        class EventHandlerCollectionBootstrap
        {
            public:
            static void BoostrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence);
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
