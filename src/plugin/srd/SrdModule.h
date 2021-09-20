#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Srd {

    extern const int srdDialogTagFunctionId;

    void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    void BootstrapRadarScreen(UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurables);
} // namespace UKControllerPlugin::Srd
