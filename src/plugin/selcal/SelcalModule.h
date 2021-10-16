#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Selcal {
    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::Selcal
