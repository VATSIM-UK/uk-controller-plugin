#include "pch/stdafx.h"
#include "releases/ReleaseModule.h"
#include "releases/EnrouteReleaseEventHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Releases {

        void BootstrapPlugin(PersistenceContainer& container)
        {
            // TODO: Get available releases
            std::shared_ptr<EnrouteReleaseEventHandler> handler = std::make_shared<EnrouteReleaseEventHandler>(
                *container.api,
                {}
            );
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
