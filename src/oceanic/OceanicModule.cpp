#include "pch/stdafx.h"
#include "oceanic/OceanicModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "oceanic/OceanicEventHandler.h"


namespace UKControllerPlugin {
    namespace Oceanic {
        void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
        {
            std::shared_ptr<OceanicEventHandler> handler = std::make_shared<OceanicEventHandler>(
                *container.curl,
                *container.taskRunner
            );

            container.timedHandler->RegisterEvent(handler, 60);
        }
    } // namespace Oceanic
} // namespace UKControllerPlugin
