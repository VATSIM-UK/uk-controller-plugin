#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::EventHandler {
    void BootstrapEventBus(const Bootstrap::PersistenceContainer& persistenceContainer);
    void ShutdownEventBus();
} // namespace UKControllerPlugin::EventHandler
