#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Aircraft {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::Aircraft
