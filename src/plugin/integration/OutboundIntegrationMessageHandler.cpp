#include "IntegrationClient.h"
#include "IntegrationClientManager.h"
#include "IntegrationConnection.h"
#include "MessageInterface.h"
#include "OutboundIntegrationMessageHandler.h"

namespace UKControllerPlugin::Integration {
    OutboundIntegrationMessageHandler::OutboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager)
        : clientManager(std::move(clientManager))
    {
    }

    void OutboundIntegrationMessageHandler::SendEvent(std::shared_ptr<MessageInterface> message) const
    {
        LogDebug("Sending integration message: " + message->ToJson().dump());
        std::for_each(
            this->clientManager->cbegin(),
            this->clientManager->cend(),
            [&message](const std::shared_ptr<IntegrationClient>& client) {
                if (!client->InterestedInMessage(message->GetMessageType())) {
                    return;
                }

                client->Connection()->Send(message);
            });
    }
} // namespace UKControllerPlugin::Integration
