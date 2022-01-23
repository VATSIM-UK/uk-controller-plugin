#include "FlightplanWakeCategoryMapper.h"
#include "WakeCategoryEventHandler.h"
#include "WakeModule.h"
#include "WakeScheme.h"
#include "WakeSchemeCollection.h"
#include "WakeSchemeCollectionFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;

namespace UKControllerPlugin::Wake {

    std::unique_ptr<WakeScheme> defaultScheme; // NOLINT
    std::unique_ptr<FlightplanWakeCategoryMapper> ukMapper;
    std::unique_ptr<FlightplanWakeCategoryMapper> recatMapper;

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
        ukMapper = std::make_unique<FlightplanWakeCategoryMapper>(
            ukScheme ? *ukScheme : *defaultScheme, *container.aircraftTypeMapper);
        recatMapper = std::make_unique<FlightplanWakeCategoryMapper>(
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
} // namespace UKControllerPlugin::Wake
