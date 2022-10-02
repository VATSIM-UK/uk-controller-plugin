#pragma once
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Runs various housekeeping functions that need to be saved for after
            everything has been connected together.
        */
        class PostInit
        {
            public:
            static void Process(const UKControllerPlugin::Bootstrap::PersistenceContainer& container);
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
