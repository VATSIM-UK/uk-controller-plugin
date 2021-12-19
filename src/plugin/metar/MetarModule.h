#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Metar {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::Metar
