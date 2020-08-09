#include "pch/stdafx.h"
#include "releases/ReleaseModule.h"
#include "releases/EnrouteReleaseEventHandler.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "releases/EnrouteReleaseTypesSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Releases {

        const std::string enrouteReleaseTypesDependency = "DEPENDENCY_ENROUTE_RELEASE_TYPES";

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
            // Load the release types
            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes;
            from_json(dependencies.LoadDependency(enrouteReleaseTypesDependency, nlohmann::json::array()), releaseTypes);

            // Create the handler
            std::shared_ptr<EnrouteReleaseEventHandler> handler = std::make_shared<EnrouteReleaseEventHandler>(
                *container.api,
                releaseTypes
            );

            // Add to events
            container.websocketProcessors->AddProcessor(handler);
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
