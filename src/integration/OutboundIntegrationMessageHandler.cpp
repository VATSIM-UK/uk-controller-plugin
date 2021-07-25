#include "pch/stdafx.h"
#include "integration/OutboundIntegrationMessageHandler.h"
#include "integration/MessageInterface.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/IntegrationClientManager.h"

namespace UKControllerPlugin::Integration {
    OutboundIntegrationMessageHandler::OutboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager): clientManager(std::move(clientManager)) {}

    void OutboundIntegrationMessageHandler::SendOutboundMessage(std::shared_ptr<MessageInterface> message) const
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

    void OutboundIntegrationMessageHandler::SendOutboundMessage(
        const std::shared_ptr<MessageInterface> message,
        int clientId
    ) const
    {
        auto client = this->clientManager->GetById(clientId);
        if (!client) {
            LogWarning("Tried to send outbound message to unknown integration id: " + clientId);
            return;
        }

        client->Connection()->Send(message);
    }
} // namespace UKControllerPlugin::Integration
