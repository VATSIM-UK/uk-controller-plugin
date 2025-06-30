#include "AircraftSelectionProvider.h"
#include "AirfieldMinimumSeparationSelectorList.h"
#include "AirfieldTargetSelectorList.h"
#include "ApproachBootstrapProvider.h"
#include "ApproachFlightplanEventHandler.h"
#include "ApproachModuleFactory.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayAsrLoader.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencerOptionsLoader.h"
#include "ApproachSpacingRingRenderer.h"
#include "GlideslopeDeviationEstimator.h"
#include "GlideslopeDeviationTagItem.h"
#include "RemoveLandedAircraft.h"
#include "SequencerAirfieldSelector.h"
#include "TargetSelectorList.h"
#include "ToggleApproachSequencerDisplay.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/PluginSettingsProviderCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "list/PopupListFactory.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Approach {

    void ApproachBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        container.pluginSettingsProviders->AddProvider(std::make_shared<ApproachSequencerOptionsLoader>(
            container.moduleFactories->Approach().SequencerOptions(), *container.airfields));

        container.timedHandler->RegisterEvent(
            std::make_shared<RemoveLandedAircraft>(
                container.moduleFactories->Approach().Sequencer(), *container.plugin),
            10);

        container.flightplanHandler->RegisterHandler(
            std::make_shared<ApproachFlightplanEventHandler>(container.moduleFactories->Approach().Sequencer()));

        // Add the deviation tag item
        const auto deviationEstimator = std::make_shared<GlideslopeDeviationEstimator>();

        container.tagHandler->RegisterTagItem(
            GLIDESLOPE_DEVIATION_TAG_ITEM_ID,
            std::make_shared<GlideslopeDeviationTagItem>(deviationEstimator, container.runwayCollection));
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
                *container.brushes,
                sequencerScreenObjectId),
            RadarScreen::RadarRenderableCollection::beforeTags);

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
            RadarScreen::RadarRenderableCollection::beforeTags);
    }
} // namespace UKControllerPlugin::Approach
