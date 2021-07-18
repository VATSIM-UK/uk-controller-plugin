#include "pch/stdafx.h"
#include "integration/IntegrationModule.h"
#include "integration/ExternalMessageEventHandler.h"
#include "integration/IntegrationServer.h"
#include "integration/ClientInitialisationManager.h"
#include "integration/IntegrationClientManager.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Integration {

        std::shared_ptr<IntegrationServer> server;
        std::shared_ptr<ClientInitialisationManager> initialisationManager;
        std::shared_ptr<IntegrationClientManager> clientManager;
        std::shared_ptr<std::thread> testThread;

        void BootstrapPlugin(PersistenceContainer& container, bool duplicatePlugin, bool winsockInitialised)
        {
            // Create handler and add to other handlers
            container.externalEventHandler = std::make_shared<ExternalMessageEventHandler>(duplicatePlugin);

            if (duplicatePlugin || !winsockInitialised) {
                return;
            }

            container.commandHandlers->RegisterHandler(container.externalEventHandler);
            container.timedHandler->RegisterEvent(container.externalEventHandler, 1);

            // Setup the new server
            clientManager = std::make_shared<IntegrationClientManager>();
            initialisationManager = std::make_shared<ClientInitialisationManager>(clientManager);
            server = std::make_shared<IntegrationServer>(initialisationManager);
            container.timedHandler->RegisterEvent(initialisationManager, 1);
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin
