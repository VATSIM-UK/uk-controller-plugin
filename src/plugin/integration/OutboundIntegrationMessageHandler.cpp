#include "IntegrationClient.h"
#include "IntegrationClientManager.h"
#include "IntegrationConnection.h"
#include "MessageInterface.h"
#include "OutboundIntegrationMessageHandler.h"
#include "log/ApiLoggerInterface.h"

namespace UKControllerPlugin::Integration {
    OutboundIntegrationMessageHandler::OutboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager)
        : clientManager(std::move(clientManager))
    {
    }

    void OutboundIntegrationMessageHandler::SendEvent(std::shared_ptr<MessageInterface> message) const
    {
        try {
            LogDebug("Sending integration message: " + message->ToJson().dump());
        } catch (const std::exception& exception) {
            if (apiLoggedTypes.find(message->GetMessageType().type) == apiLoggedTypes.end()) {
                LogError(
                    "Failed to log integration message, something's wrong with the JSON: " +
                    message->GetMessageType().type);
                std::string messageType = message->GetMessageType().type;

                // Add the message type to the set so we don't log it again
                apiLoggedTypes.insert(messageType);

                const auto metadata = nlohmann::json{
                    {"json_without_strict",
                     message->ToJson().dump(-1, ' ', false, nlohmann::json::error_handler_t::ignore)},
                    {"exception", exception.what()}};

                ApiLogger().Log("INTEGRATION_INVALID_JSON", "Failed to log integration message", metadata);
            };

            // We'll just have to accept that we can't send this message to integrations
            return;
        }

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
