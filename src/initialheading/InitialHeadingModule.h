#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace EventHandler {
        class FlightPlanEventHandlerCollection;
    }  // namespace EventHandler
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace InitialHeading {

        void BootstrapPlugin(Bootstrap::PersistenceContainer& persistence);
    } // namespace InitialHeading
}  // namespace UKControllerPlugin
