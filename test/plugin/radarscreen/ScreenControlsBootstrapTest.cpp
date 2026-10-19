#include "radarscreen/ScreenControlsBootstrap.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::RadarScreen::ScreenControlsBootstrap;

namespace UKControllerPluginTest {
    namespace RadarScreen {

        TEST(ScreenControlsBootstrap, BootstrapRadarScreenAddsToRenderables)
        {
            ConfigurableDisplayCollection displays;
            RadarRenderableCollection radarRenderable;
            ScreenControlsBootstrap::BootstrapRadarScreen(displays, radarRenderable);
            EXPECT_EQ(1, radarRenderable.CountRenderers());
            EXPECT_EQ(1, radarRenderable.CountRenderersInPhase(radarRenderable.beforeTags));
        }

        TEST(ScreenControlsBootstrap, BootstrapRadarScreenRegistersScreenObjects)
        {
            ConfigurableDisplayCollection displays;
            RadarRenderableCollection radarRenderable;
            ScreenControlsBootstrap::BootstrapRadarScreen(displays, radarRenderable);
            EXPECT_EQ(1, radarRenderable.CountScreenObjects());
        }
    } // namespace RadarScreen
} // namespace UKControllerPluginTest
