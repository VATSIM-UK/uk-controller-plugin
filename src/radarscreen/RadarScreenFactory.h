#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    class UKRadarScreen;
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            A class to create a RadarScreen object.
        */
        class RadarScreenFactory
        {
            public:
                explicit RadarScreenFactory(const UKControllerPlugin::Bootstrap::PersistenceContainer & persistence);
                UKControllerPlugin::UKRadarScreen * Create(void) const;

            private:
                const UKControllerPlugin::Bootstrap::PersistenceContainer & persistence;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
