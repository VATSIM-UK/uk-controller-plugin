#include "pch/stdafx.h"
#include "integration/OutboundIntegrationMessageHandler.h"
#include "integration/MessageInterface.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/IntegrationClientManager.h"

namespace UKControllerPlugin::Integration {
    OutboundIntegrationMessageHandler::OutboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager): clientManager(std::move(clientManager)) {}

    void OutboundIntegrationMessageHandler::SendEvent(std::shared_ptr<MessageInterface> message)
    {
        std::for_each(
            this->clientManager->cbegin(),
            this->clientManager->cend(),
            [&message](const std::shared_ptr<IntegrationClient>& client)
            {
                if (!client->InterestedInMessage(message->GetMessageType())) {
                    return;
                }

                client->Connection()->Send(message);
            }
        );
    }
} // namespace UKControllerPlugin::Integration
