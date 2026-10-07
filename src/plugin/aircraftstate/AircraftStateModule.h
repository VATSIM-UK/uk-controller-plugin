#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::AircraftState {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
} // namespace UKControllerPlugin::AircraftState
