#pragma once
#include "pch/stdafx.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents a message that's inbound from an integration.
     */
    class InboundMessage : public MessageInterface
    {
        public:
            InboundMessage(MessageType type, nlohmann::json data): type(type), data(data) {}
            ~InboundMessage() override = default;

            /*
             * Get the data to go in the message.
             */
            nlohmann::json GetMessageData() const override
            {
                return this->data;
            }

            /*
             * Get the type of the message.
             */
            MessageType GetMessageType() const override
            {
                return this->type;
            }

        private:
            // The type of message
            MessageType type;

            // Data about the message
            nlohmann::json data;
    };
} // namespace UKControllerPlugin::Integration
