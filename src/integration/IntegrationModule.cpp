#include "pch/stdafx.h"
#include "integration/IntegrationModule.h"
#include "integration/OutboundIntegrationMessageHandler.h"
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
        std::shared_ptr<OutboundIntegrationMessageHandler> outboundHandler;

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
            container.timedHandler->RegisterEvent(clientManager, 5);
            initialisationManager = std::make_shared<ClientInitialisationManager>(clientManager);
            server = std::make_shared<IntegrationServer>(initialisationManager);
            outboundHandler = std::make_shared<OutboundIntegrationMessageHandler>(clientManager);
            container.timedHandler->RegisterEvent(initialisationManager, 1);

            container.integrationMessageHandlers = std::make_shared<InboundIntegrationMessageHandler>(clientManager);
            container.timedHandler->RegisterEvent(container.integrationMessageHandlers, 1);
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin
