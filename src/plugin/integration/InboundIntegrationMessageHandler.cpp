#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageInterface.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationActionProcessor.h"
#include "integration/ActionFailureMessage.h"
#include "integration/ActionSuccessMessage.h"

namespace UKControllerPlugin::Integration {
    InboundIntegrationMessageHandler::InboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager): clientManager(std::move(clientManager)) {}

    void InboundIntegrationMessageHandler::AddProcessor(std::shared_ptr<IntegrationActionProcessor> processor)
    {
        for (const MessageType& actionType : processor->ActionsToProcess()) {
            if (this->actionProcessors.count(actionType)) {
                LogError("An action processor already exists for type " + actionType.ToJson().dump());
                return;
            }
            
            this->actionProcessors[actionType] = processor;
        }
    }

    /*
     * Every time the timed event triggers, process inbound messages
     */
    void InboundIntegrationMessageHandler::TimedEventTrigger()
    {
        for (
            auto client = this->clientManager->cbegin();
            client != this->clientManager->cend();
            ++client
        ) {
            this->ProcessInboundMessagesForClient(*client);
        }
    }

    size_t InboundIntegrationMessageHandler::CountProcessors() const
    {
        return this->actionProcessors.size();
    }

    /*
     * For a given client, process all their inbound messages
     */
    void InboundIntegrationMessageHandler::ProcessInboundMessagesForClient(
        const std::shared_ptr<IntegrationClient>& client
    )
    {
        auto messages = client->Connection()->Receive();
        while (!messages.empty()) {
            auto message = messages.front();
            if (!this->actionProcessors.count(message->GetMessageType())) {
                LogNoProcessorExists(client, message->GetMessageType());
                messages.pop();
                continue;
            }

            this->actionProcessors[message->GetMessageType()]->ProcessAction(
                message,
                [client, message]()
                {
                    client->Connection()->Send(
                        std::make_shared<ActionSuccessMessage>(message->GetMessageId())
                    );
                },
                [client, message](std::vector<std::string> errors) {
                    client->Connection()->Send(
                        std::make_shared<ActionFailureMessage>(message->GetMessageId(), std::move(errors))
                    );
                }
            );
            messages.pop();
        }
    }

    void InboundIntegrationMessageHandler::LogNoProcessorExists(
        const std::shared_ptr<IntegrationClient>& client,
        const MessageType& messageType
    )
    {
        LogError(
            "No message processor exists for type " + messageType.ToJson().dump() +
            " from integration " + client->GetIntegrationString()
        );
    }

} // namespace UKControllerPlugin::Integration
