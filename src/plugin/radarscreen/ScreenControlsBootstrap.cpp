#include "radarscreen/ScreenControlsBootstrap.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ScreenControls.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ScreenControls;
using UKControllerPlugin::Windows::GdiplusBrushes;
namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            Bootstraps the radar screen aspect of this module
        */
        void ScreenControlsBootstrap::BootstrapRadarScreen(
            ConfigurableDisplayCollection configurableDisplays,
            RadarRenderableCollection& radarRenderables,
            const GdiplusBrushes& brushes)
        {
            // Add it to the renderables and reserve a clickspot for it
            int rendererId = radarRenderables.ReserveRendererIdentifier();
            radarRenderables.RegisterRenderer(
                rendererId,
                std::make_shared<ScreenControls>(
                    radarRenderables.ReserveScreenObjectIdentifier(rendererId), configurableDisplays, brushes),
                radarRenderables.beforeTags);
        }
    } // namespace RadarScreen
} // namespace UKControllerPlugin
