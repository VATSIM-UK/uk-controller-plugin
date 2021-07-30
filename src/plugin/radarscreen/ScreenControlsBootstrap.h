#pragma once
#include "radarscreen/ConfigurableDisplayCollection.h"

// Forward declare
namespace UKControllerPlugin {
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Windows {
        struct GdiplusBrushes;
    }  // namespace Windows
}  // namespace UKControllerPlugin
// END

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
                    UKControllerPlugin::RadarScreen::RadarRenderableCollection & radarRenderables,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes
                );
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
