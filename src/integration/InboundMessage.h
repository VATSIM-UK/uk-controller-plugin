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
            ~InboundMessage() override = default;
            nlohmann::json GetMessageData() const override;
            MessageType GetMessageType() const override;
            static bool MessageFormatValid(const nlohmann::json& message);
            static std::shared_ptr<InboundMessage> FromJson(const nlohmann::json& json);

        protected:
            InboundMessage(MessageType type, nlohmann::json::object_t data): type(type), data(data) {}

        private:
            // The type of message
            MessageType type;

            // Data about the message
            nlohmann::json::object_t data;
    };
} // namespace UKControllerPlugin::Integration
