#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Euroscope {
        struct AsrEventHandlerCollection;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::SectorFile {

    void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

    void BootstrapRadarScreen(
        const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
        UKControllerPlugin::Euroscope::AsrEventHandlerCollection& asrEvents);
} // namespace UKControllerPlugin::SectorFile
