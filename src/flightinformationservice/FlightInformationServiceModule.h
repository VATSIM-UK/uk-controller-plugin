#pragma once

namespace UKControllerPlugin {
    // Forward declarations
    namespace Bootstrap
    {
        struct PersistenceContainer;
    }

    namespace FlightInformationService {
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
