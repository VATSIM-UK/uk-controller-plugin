#pragma once
#include "radarscreen/RadarRenderableCollection.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    class UKRadarScreen;
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin::RadarScreen {

    /*
        A class to create a RadarScreen object.
    */
    class RadarScreenFactory
    {
        public:
        explicit RadarScreenFactory(const UKControllerPlugin::Bootstrap::PersistenceContainer& persistence);
        UKControllerPlugin::UKRadarScreen* Create() const;

        private:
        // Container of all the things
        const UKControllerPlugin::Bootstrap::PersistenceContainer& persistence;

        // Stores the renderables
        mutable std::vector<std::shared_ptr<UKControllerPlugin::RadarScreen::RadarRenderableCollection>>
            renderableCollections;
    };
} // namespace UKControllerPlugin::RadarScreen
