#pragma once
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {
    class MessageInterface;

    /*
     * An interface for classes that wish to process inbound messages from integrations.
     */
    class InboundIntegrationMessageProcessor
    {
        public:
            virtual ~InboundIntegrationMessageProcessor();
            virtual MessageType MessageToProcess() const = 0;
            virtual void ProcessMessage(std::shared_ptr<MessageInterface> message) = 0;
    };
} // namespace UKControllerPlugin::Integration
