#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {
    class IntegrationClient;
    class IntegrationClientManager;
    class IntegrationActionProcessor;

    /*
     * A class dedicated to handling inbound messages from integrations
     * to the plugin.
     */
    class InboundIntegrationMessageHandler : public TimedEvent::AbstractTimedEvent
    {
        public:
        explicit InboundIntegrationMessageHandler(std::shared_ptr<IntegrationClientManager> clientManager);
        ~InboundIntegrationMessageHandler() override = default;
        void AddProcessor(std::shared_ptr<IntegrationActionProcessor> processor);
        void TimedEventTrigger() override;
        size_t CountProcessors() const;

        private:
        void ProcessInboundMessagesForClient(const std::shared_ptr<IntegrationClient>& client);
        static void
        LogNoProcessorExists(const std::shared_ptr<IntegrationClient>& client, const MessageType& messageType);

        // Manages clients
        const std::shared_ptr<IntegrationClientManager> clientManager;

        // Processors for inbound messages
        std::map<MessageType, std::shared_ptr<IntegrationActionProcessor>> actionProcessors;
    };
} // namespace UKControllerPlugin::Integration
