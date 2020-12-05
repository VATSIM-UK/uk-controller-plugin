#include "pch/stdafx.h"
#include "integration/IntegrationModule.h"
#include "integration/ExternalMessageEventHandler.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Integration {

        void BootstrapPlugin(PersistenceContainer& container, bool duplicatePlugin)
        {
            if (duplicatePlugin) {
                return;
            }

            // Create handler and add to other handlers
            container.externalEventHandler = std::make_shared<ExternalMessageEventHandler>();
            container.commandHandlers->RegisterHandler(container.externalEventHandler);
            container.timedHandler->RegisterEvent(container.externalEventHandler, 1);
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin
