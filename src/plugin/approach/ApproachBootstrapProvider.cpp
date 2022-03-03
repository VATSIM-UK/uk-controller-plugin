#include "ApproachBootstrapProvider.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSpacingRingRenderer.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/RadarRenderableCollection.h"

namespace UKControllerPlugin::Approach {

    void ApproachBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
    }

    void ApproachBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers)
    {
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<ApproachSpacingRingRenderer>(*container.plugin),
            radarRenderables.beforeTags);

        auto sequencerRendererId = radarRenderables.ReserveRendererIdentifier();
        auto sequencerScreenObjectId = radarRenderables.ReserveScreenObjectIdentifier(sequencerRendererId);

        radarRenderables.RegisterRenderer(
            sequencerRendererId,
            std::make_shared<ApproachSequencerDisplay>(sequencerScreenObjectId),
            radarRenderables.beforeTags);
    }
} // namespace UKControllerPlugin::Approach
