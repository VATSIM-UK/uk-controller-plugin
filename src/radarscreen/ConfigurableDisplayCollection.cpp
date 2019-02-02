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
            if (std::find(this->displays.begin(), this->displays.end(), radarScreen) != this->displays.end()) {
                LogWarning("Duplicate configurable display added");
                return;
            }

            this->displays.push_back(radarScreen);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
