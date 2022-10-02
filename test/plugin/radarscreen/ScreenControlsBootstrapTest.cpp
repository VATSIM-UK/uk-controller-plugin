#include "radarscreen/ScreenControlsBootstrap.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::RadarScreen::ScreenControlsBootstrap;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPluginTest {
    namespace RadarScreen {

        TEST(ScreenControlsBootstrap, BootstrapRadarScreenAddsToRenderables)
        {
            ConfigurableDisplayCollection displays;
            RadarRenderableCollection radarRenderable;
            GdiplusBrushes brushes;
            ScreenControlsBootstrap::BootstrapRadarScreen(displays, radarRenderable, brushes);
            EXPECT_EQ(1, radarRenderable.CountRenderers());
            EXPECT_EQ(1, radarRenderable.CountRenderersInPhase(radarRenderable.beforeTags));
        }

        TEST(ScreenControlsBootstrap, BootstrapRadarScreenRegistersScreenObjects)
        {
            ConfigurableDisplayCollection displays;
            RadarRenderableCollection radarRenderable;
            GdiplusBrushes brushes;
            ScreenControlsBootstrap::BootstrapRadarScreen(displays, radarRenderable, brushes);
            EXPECT_EQ(1, radarRenderable.CountScreenObjects());
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
