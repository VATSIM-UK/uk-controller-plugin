#include "pch/stdafx.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            Returns the number of displays.
        */
        size_t ConfigurableDisplayCollection::CountDisplays(void) const
        {
            return this->displays.size();
        }

        /*
            Adds a display to the collection.
        */
        void ConfigurableDisplayCollection::RegisterDisplay(
            std::shared_ptr<UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface> radarScreen
        ) {
            this->displays.insert(radarScreen);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
