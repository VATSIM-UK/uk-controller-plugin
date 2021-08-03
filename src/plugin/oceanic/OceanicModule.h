#pragma once

namespace UKControllerPlugin {

    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap

    namespace Oceanic {
        void BootstrapPlugin(const Bootstrap::PersistenceContainer& container);
    } // namespace Oceanic
} // namespace UKControllerPlugin
