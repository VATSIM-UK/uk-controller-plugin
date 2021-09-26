#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::MissedApproach {
    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::MissedApproach
