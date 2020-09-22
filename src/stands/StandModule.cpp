#pragma once
#include "pch/stdafx.h"
#include "stands/StandModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "stands/StandEventHandler.h"
#include "stands/Stand.h"
#include "stands/CompareStands.h"
#include "stands/StandSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Stands {

        // The tag item id for assigned stand
        const int assignedStandTagItemId = 110;

        const std::string standDependency = "DEPENDENCY_STANDS";

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
            // Load stand data from the dependency
            std::set<Stand, CompareStands> stands;
            from_json(
                dependencies.LoadDependency(standDependency, nlohmann::json::object()),
                stands
            );

            // Create the event handler
            std::shared_ptr<StandEventHandler> eventHandler = std::make_shared<StandEventHandler>(
                stands
            );

            // Assign to handlers
            container.tagHandler->RegisterTagItem(assignedStandTagItemId, eventHandler);
            container.websocketProcessors->AddProcessor(eventHandler);
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
