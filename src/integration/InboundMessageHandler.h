#pragma once
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {

    class OutboundMessageTarget;
    class MessageInterface;

    /*
     * Registers to handle particular types of inbound messages
     * and handles them.
     */
    class InboundMessageHandler
    {
        public:
            virtual ~InboundMessageHandler() = default;
            virtual std::vector<MessageType> GetMessageSubscriptions() const = 0;
            virtual bool HandleMessage(std::shared_ptr<MessageInterface> message) = 0;
    };
} // namespace UKControllerPlugin::Integration
