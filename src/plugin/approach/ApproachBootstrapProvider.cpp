#include "ApproachBootstrapProvider.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayAsrLoader.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSpacingRingRenderer.h"
#include "SequencerAirfieldSelector.h"
#include "ToggleApproachSequencerDisplay.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "list/PopupListFactory.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"

namespace UKControllerPlugin::Approach {

    void ApproachBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
    }

    void ApproachBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        // Ring renderer
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<ApproachSpacingRingRenderer>(*container.plugin),
            radarRenderables.beforeTags);

        // Sequencer display
        auto sequencerRendererId = radarRenderables.ReserveRendererIdentifier();
        auto sequencerScreenObjectId = radarRenderables.ReserveScreenObjectIdentifier(sequencerRendererId);

        auto displayOptions = std::make_shared<ApproachSequencerDisplayOptions>();
        radarRenderables.RegisterRenderer(
            sequencerRendererId,
            std::make_shared<ApproachSequencerDisplay>(
                displayOptions,
                container.popupListFactory->Create(
                    std::make_shared<SequencerAirfieldSelector>(displayOptions, *container.airfields),
                    "Toggle sequencer airfield selector"),
                sequencerScreenObjectId),
            radarRenderables.beforeTags);

        asrHandlers.RegisterHandler(std::make_shared<ApproachSequencerDisplayAsrLoader>(displayOptions));
        toggleableDisplayFactory.RegisterDisplay(
            std::make_shared<ToggleApproachSequencerDisplay>(displayOptions), "Approach sequencer display toggle");
    }
} // namespace UKControllerPlugin::Approach
