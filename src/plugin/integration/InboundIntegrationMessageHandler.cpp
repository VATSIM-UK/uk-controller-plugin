#include "pch/pch.h"
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageInterface.h"
#include "integration/IntegrationClientManager.h"
#include "integration/InboundIntegrationMessageProcessor.h"

namespace UKControllerPlugin::Integration {
    InboundIntegrationMessageHandler::InboundIntegrationMessageHandler(
        std::shared_ptr<IntegrationClientManager> clientManager): clientManager(std::move(clientManager)) {}

    void InboundIntegrationMessageHandler::AddProcessor(std::shared_ptr<InboundIntegrationMessageProcessor> processor)
    {
        auto messageType = processor->MessageToProcess();
        if (this->messageProcessors.count(messageType)) {
            LogError("A message processor already exists for type " + messageType.ToJson().dump());
            return;
        }

        this->messageProcessors[messageType] = processor;
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
        return this->messageProcessors.size();
    }

    /*
     * For a given client, process all their inbound messages
     */
    void InboundIntegrationMessageHandler::ProcessInboundMessagesForClient(std::shared_ptr<IntegrationClient> client)
    {
        auto messages = client->Connection()->Receive();
        while (!messages.empty()) {
            auto message = messages.front();
            if (!this->messageProcessors.count(message->GetMessageType())) {
                LogNoProcessorExists(client, message->GetMessageType());
                messages.pop();
                continue;
            }

            this->messageProcessors[message->GetMessageType()]->ProcessMessage(
                message,
                [client](std::shared_ptr<MessageInterface> message)
                {
                    client->Connection()->Send(message);
                }
            );
            messages.pop();
        }
    }

    void InboundIntegrationMessageHandler::LogNoProcessorExists(
        std::shared_ptr<IntegrationClient> client,
        MessageType messageType
    )
    {
        LogError(
            "No message processor exists for type " + messageType.ToJson().dump() +
            " from integration " + client->GetIntegrationString()
        );
    }

} // namespace UKControllerPlugin::Integration
