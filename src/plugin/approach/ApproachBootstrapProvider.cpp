#include "AircraftSelectionProvider.h"
#include "AirfieldTargetSelectorList.h"
#include "AirfieldMinimumSeparationSelectorList.h"
#include "ApproachBootstrapProvider.h"
#include "ApproachModuleFactory.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayAsrLoader.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencerOptionsLoader.h"
#include "ApproachSpacingRingRenderer.h"
#include "RemoveLandedAircraft.h"
#include "SequencerAirfieldSelector.h"
#include "TargetSelectorList.h"
#include "ToggleApproachSequencerDisplay.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/PluginSettingsProviderCollection.h"
#include "list/PopupListFactory.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::Approach {

    void ApproachBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        container.pluginSettingsProviders->AddProvider(std::make_shared<ApproachSequencerOptionsLoader>(
            container.moduleFactories->Approach().SequencerOptions(), *container.airfields));

        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveLandedAircraft>(
                container.moduleFactories->Approach().Sequencer(), *container.plugin),
            10);
    }

    void ApproachBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        // Sequencer display
        auto sequencerRendererId = radarRenderables.ReserveRendererIdentifier();
        auto sequencerScreenObjectId = radarRenderables.ReserveScreenObjectIdentifier(sequencerRendererId);

        auto displayOptions = std::make_shared<ApproachSequencerDisplayOptions>();
        radarRenderables.RegisterRenderer(
            sequencerRendererId,
            std::make_shared<ApproachSequencerDisplay>(
                container.moduleFactories->Approach().Sequencer(),
                container.moduleFactories->Approach().SpacingCalculator(container),
                container.moduleFactories->Approach().SequencerOptions(),
                displayOptions,
                container.popupListFactory->Create(
                    std::make_shared<SequencerAirfieldSelector>(displayOptions, *container.airfields),
                    "Toggle sequencer airfield selector"),
                container.callsignSelectionListFactory->Create(
                    std::make_shared<AircraftSelectionProvider>(
                        container.moduleFactories->Approach().Sequencer(),
                        container.moduleFactories->Approach().SequencerOptions(),
                        displayOptions,
                        *container.plugin),
                    "Toggle sequencer callsign selector"),
                container.popupListFactory->Create(
                    std::make_shared<TargetSelectorList>(
                        container.moduleFactories->Approach().Sequencer(), displayOptions, *container.plugin),
                    "Toggle sequencer target selector"),
                container.popupListFactory->Create(
                    std::make_shared<AirfieldTargetSelectorList>(
                        container.moduleFactories->Approach().SequencerOptions(), displayOptions),
                    "Toggle sequencer airfield target selector"),
                container.popupListFactory->Create(
                    std::make_shared<AirfieldMinimumSeparationSelectorList>(
                        container.moduleFactories->Approach().SequencerOptions(), displayOptions),
                    "Toggle sequencer airfield separation selector"),
                *container.plugin,
                sequencerScreenObjectId),
            radarRenderables.beforeTags);

        asrHandlers.RegisterHandler(std::make_shared<ApproachSequencerDisplayAsrLoader>(displayOptions));
        toggleableDisplayFactory.RegisterDisplay(
            std::make_shared<ToggleApproachSequencerDisplay>(displayOptions), "Approach sequencer display toggle");

        // Ring renderer
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<ApproachSpacingRingRenderer>(
                container.moduleFactories->Approach().Sequencer(),
                container.moduleFactories->Approach().SpacingCalculator(container),
                displayOptions,
                *container.plugin),
            radarRenderables.beforeTags);
    }
} // namespace UKControllerPlugin::Approach
