#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::List {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::List
