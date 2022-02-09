#include "aircraft/CallsignSelectionListFactory.h"
#include "FlightplanWakeCategoryMapper.h"
#include "FollowingWakeCallsignProvider.h"
#include "LeadWakeCallsignProvider.h"
#include "WakeCalculatorDisplay.h"
#include "WakeCalculatorOptions.h"
#include "WakeCategoryEventHandler.h"
#include "WakeModule.h"
#include "WakeScheme.h"
#include "WakeSchemeCollection.h"
#include "WakeSchemeCollectionFactory.h"
#include "WakeSchemeProvider.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "list/PopupList.h"
#include "list/PopupListFactory.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;

namespace UKControllerPlugin::Wake {

    std::unique_ptr<WakeScheme> defaultScheme; // NOLINT
    std::shared_ptr<FlightplanWakeCategoryMapper> ukMapper;
    std::shared_ptr<FlightplanWakeCategoryMapper> recatMapper;

    /*
        Bootstrap everything
    */
    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
    {
        // Create the wake categories
        container.wakeSchemes =
            CollectionFromDependency(dependencies.LoadDependency("DEPENDENCY_WAKE_SCHEME", nlohmann::json::array()));

        defaultScheme =
            std::make_unique<WakeScheme>(-1, "DEF", "UK Default", std::list<std::shared_ptr<WakeCategory>>{});

        // Create the mappers
        const auto ukScheme = container.wakeSchemes->GetByKey("UK");
        const auto recatScheme = container.wakeSchemes->GetByKey("RECAT_EU");
        ukMapper = std::make_shared<FlightplanWakeCategoryMapper>(
            ukScheme ? *ukScheme : *defaultScheme, *container.aircraftTypeMapper);
        recatMapper = std::make_shared<FlightplanWakeCategoryMapper>(
            recatScheme ? *recatScheme : *defaultScheme, *container.aircraftTypeMapper);

        // Create handler and register
        auto handler = std::make_shared<WakeCategoryEventHandler>(*ukMapper, *recatMapper);

        container.flightplanHandler->RegisterHandler(handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdAircraftTypeCategory, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdStandaloneCategory, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdRecat, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdUkRecatCombined, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdAircraftTypeRecat, handler);
    }

    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& renderables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        auto options = std::make_shared<WakeCalculatorOptions>();
        options->Scheme("UK");
        options->SchemeMapper(ukMapper);

        const auto rendererId = renderables.ReserveRendererIdentifier();
        const auto renderer = std::make_shared<WakeCalculatorDisplay>(
            options,
            container.callsignSelectionListFactory->Create(
                std::make_shared<LeadWakeCallsignProvider>(*container.airfieldOwnership, *container.plugin, options),
                "Wake Calculator Lead Aircraft"),
            container.callsignSelectionListFactory->Create(
                std::make_shared<FollowingWakeCallsignProvider>(*container.plugin, options),
                "Wake Calculator Following Aircraft"),
            container.popupListFactory->Create(
                std::make_shared<WakeSchemeProvider>(options, *container.wakeSchemes, *container.aircraftTypeMapper),
                "Wake Calculator Scheme"),
            *container.plugin,
            renderables.ReserveScreenObjectIdentifier(rendererId));

        renderables.RegisterRenderer(rendererId, renderer, RadarScreen::RadarRenderableCollection::afterLists);
        asrHandlers.RegisterHandler(renderer);
        toggleableDisplayFactory.RegisterDisplay(renderer, "Toggle Wake Turbulence Calculator");
    }
} // namespace UKControllerPlugin::Wake
