#pragma once
#include "MessageInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents a message that's inbound from an integration.
     */
    class InboundMessage : public MessageInterface
    {
        public:
            [[nodiscard]] auto GetMessageData() const -> nlohmann::json override;
            [[nodiscard]] auto GetMessageType() const -> MessageType override;
            [[nodiscard]] auto GetMessageId() const -> std::string override;
            static auto FromJson(const nlohmann::json& json) -> std::shared_ptr<InboundMessage>;
            
        protected:
            static auto MessageFormatValid(const nlohmann::json& message) -> bool;
            InboundMessage(std::string id, MessageType type, nlohmann::json::object_t data)
                : type(std::move(type)), data(std::move(data)), id(std::move(id)) {}
    
        private:
            // The type of message
            MessageType type;

            // Data about the message
            nlohmann::json::object_t data;
            
            // The id of the message
            std::string id;
    };
} // namespace UKControllerPlugin::Integration
