#include "pch/stdafx.h"
#include "wake/WakeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "wake/CreateWakeMappings.h"
#include "WakeCategoryEventHandler.h"
#include "dependency/DependencyLoaderInterface.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Wake::CreateWakeMappings;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;

namespace UKControllerPlugin {
    namespace Wake {

        const std::string dependencyFile = "DEPENDENCY_WAKE";
        const int tagItemId = 105;

        /*
            Bootstrap everything
        */
        void BootstrapPlugin(const PersistenceContainer & container, DependencyLoaderInterface& dependencies)
        {
            // Create the data
            nlohmann::json data = dependencies.LoadDependency(dependencyFile, nlohmann::json::object());

            // Create handler and register
            std::shared_ptr<WakeCategoryEventHandler> handler = std::make_shared<WakeCategoryEventHandler>(
                CreateWakeMappings(data, *container.userMessager)
            );

            container.flightplanHandler->RegisterHandler(handler);
            container.tagHandler->RegisterTagItem(tagItemId, handler);
        }

    }  // namespace Wake
}  // namespace UKControllerPlugin
