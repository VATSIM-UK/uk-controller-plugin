#pragma once
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            Class for bootstrapping the ScreenControls renderer
        */
        class ScreenControlsBootstrap
        {
            public:
            static void BootstrapRadarScreen(
                UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection configurableDisplays,
                UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRenderables);
        };
    } // namespace RadarScreen
} // namespace UKControllerPlugin
