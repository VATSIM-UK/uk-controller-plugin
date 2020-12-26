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
        const std::string recatDependencyFile = "DEPENDENCY_RECAT";
        const int tagItemId = 105;
        const int tagItemRecatId = 112;
        const int tagItemStandaloneId = 113;

        /*
            Bootstrap everything
        */
        void BootstrapPlugin(const PersistenceContainer & container, DependencyLoaderInterface& dependencies)
        {
            // Create the data
            nlohmann::json data = dependencies.LoadDependency(dependencyFile, nlohmann::json::object());
            nlohmann::json recatData = dependencies.LoadDependency(recatDependencyFile, nlohmann::json::object());

            // Create handler and register
            std::shared_ptr<WakeCategoryEventHandler> handler = std::make_shared<WakeCategoryEventHandler>(
                CreateWakeMappings(
                    data,
                    *container.userMessager
                ),
                CreateWakeMappings(
                    recatData,
                    *container.userMessager,
                    "RECAT"
                )
            );

            container.flightplanHandler->RegisterHandler(handler);
            container.tagHandler->RegisterTagItem(tagItemId, handler);
            container.tagHandler->RegisterTagItem(tagItemRecatId, handler);
            container.tagHandler->RegisterTagItem(tagItemStandaloneId, handler);
        }

    }  // namespace Wake
}  // namespace UKControllerPlugin
