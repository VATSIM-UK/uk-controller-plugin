#include "CreateWakeMappings.h"
#include "WakeCategoryEventHandler.h"
#include "WakeModule.h"
#include "WakeSchemeCollection.h"
#include "WakeSchemeCollectionFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Wake::CreateWakeMappings;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;

namespace UKControllerPlugin::Wake {
    /*
        Bootstrap everything
    */
    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
    {
        // Create the wake categories
        container.wakeSchemes =
            CollectionFromDependency(dependencies.LoadDependency("DEPENDENCY_WAKE_SCHEME", nlohmann::json::array()));

        // Create the data
        nlohmann::json data = dependencies.LoadDependency(GetWakeDependencyKey(), nlohmann::json::object());
        nlohmann::json recatData = dependencies.LoadDependency(GetRecatDependencyKey(), nlohmann::json::object());

        // Create handler and register
        std::shared_ptr<WakeCategoryEventHandler> handler = std::make_shared<WakeCategoryEventHandler>(
            CreateWakeMappings(data, *container.userMessager),
            CreateWakeMappings(recatData, *container.userMessager, "RECAT"));

        container.flightplanHandler->RegisterHandler(handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdAircraftTypeCategory, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdStandaloneCategory, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdRecat, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdUkRecatCombined, handler);
        container.tagHandler->RegisterTagItem(handler->tagItemIdAircraftTypeRecat, handler);
    }

    auto GetWakeDependencyKey() -> std::string
    {
        return "DEPENDENCY_WAKE";
    }

    auto GetRecatDependencyKey() -> std::string
    {
        return "DEPENDENCY_RECAT";
    }
} // namespace UKControllerPlugin::Wake
