#pragma once
#include <utility>

#include "pch/pch.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents a message that's inbound from an integration.
     */
    class InboundMessage : public MessageInterface
    {
        public:
            ~InboundMessage() override = default;
            [[nodiscard]] nlohmann::json GetMessageData() const override;
            [[nodiscard]] MessageType GetMessageType() const override;
            [[nodiscard]] std::string GetMessageId() const override;
            static std::shared_ptr<InboundMessage> FromJson(const nlohmann::json& json);
            
        protected:
            static bool MessageFormatValid(const nlohmann::json& message);
            InboundMessage(std::string id, MessageType type, nlohmann::json::object_t data)
                : id(std::move(id)), type(std::move(type)), data(std::move(data)) {}
    
        private:
            // The type of message
            MessageType type;

            // Data about the message
            nlohmann::json::object_t data;
            
            // The id of the message
            std::string id;
    };
} // namespace UKControllerPlugin::Integration
