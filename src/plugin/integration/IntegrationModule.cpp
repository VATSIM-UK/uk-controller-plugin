#include "ClientInitialisationManager.h"
#include "DummyOutboundIntegrationMessageHandler.h"
#include "ExternalMessageEventHandler.h"
#include "InboundIntegrationMessageHandler.h"
#include "IntegrationClientManager.h"
#include "IntegrationDataInitialisers.h"
#include "IntegrationModule.h"
#include "IntegrationPersistenceContainer.h"
#include "IntegrationServer.h"
#include "OutboundIntegrationMessageHandler.h"
#include "command/CommandHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Integration {

    const int clientManagerTimedEventFrequency = 5;

    void BootstrapPlugin(PersistenceContainer& container, bool duplicatePlugin, bool winsockInitialised)
    {
        // Create handler and add to other handlers
        container.externalEventHandler = std::make_shared<ExternalMessageEventHandler>(duplicatePlugin);
        auto clientManager = std::make_shared<IntegrationClientManager>();
        auto inboundMessageProcessors = std::make_shared<InboundIntegrationMessageHandler>(clientManager);
        auto dataInitialisers = std::make_shared<IntegrationDataInitialisers>();

        if (duplicatePlugin || !winsockInitialised) {
            container.integrationModuleContainer = std::make_unique<IntegrationPersistenceContainer>(
                std::make_shared<DummyOutboundIntegrationMessageHandler>(),
                std::move(inboundMessageProcessors),
                nullptr,
                std::move(dataInitialisers));
            return;
        }

        container.timedHandler->RegisterEvent(clientManager, clientManagerTimedEventFrequency);
        container.timedHandler->RegisterEvent(inboundMessageProcessors, 1);
        container.commandHandlers->RegisterHandler(container.externalEventHandler);
        container.timedHandler->RegisterEvent(container.externalEventHandler, 1);

        // Setup the new server
        auto initialisationManager = std::make_shared<ClientInitialisationManager>(clientManager, dataInitialisers);
        auto server = std::make_unique<IntegrationServer>(initialisationManager);
        container.timedHandler->RegisterEvent(initialisationManager, 1);
        const auto outboundHandler = std::make_shared<OutboundIntegrationMessageHandler>(clientManager);

        container.integrationModuleContainer = std::make_unique<IntegrationPersistenceContainer>(
            outboundHandler, std::move(inboundMessageProcessors), std::move(server), std::move(dataInitialisers));
    }
} // namespace UKControllerPlugin::Integration
