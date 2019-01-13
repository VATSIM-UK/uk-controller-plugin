#include "pch/stdafx.h"
#include "wake/WakeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "wake/CreateWakeMappings.h"
#include "WakeCategoryEventHandler.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Wake::CreateWakeMappings;
using UKControllerPlugin::Wake::WakeCategoryEventHandler;

namespace UKControllerPlugin {
    namespace Wake {

        const std::string dependencyFile = "wake-categories.json";
        const int tagItemId = 105;

        /*
            Bootstrap everything
        */
        void BootstrapPlugin(const PersistenceContainer & container, const DependencyCache & dependencies)
        {
            // Create the data
            nlohmann::json data;
            try {
                data = nlohmann::json::parse(dependencies.GetDependency(dependencyFile));
            } catch (nlohmann::json::exception) {
                LogError("Error wake categories file, invalid JSON");
            } catch (...) {
                LogError("GEneral area parsing wake categories file");
            }

            // Create handler and register
            std::shared_ptr<WakeCategoryEventHandler> handler = std::make_shared<WakeCategoryEventHandler>(
                CreateWakeMappings(data, *container.userMessager)
            );

            container.flightplanHandler->RegisterHandler(handler);
            container.tagHandler->RegisterTagItem(tagItemId, handler);
        }

    }  // namespace Wake
}  // namespace UKControllerPlugin
