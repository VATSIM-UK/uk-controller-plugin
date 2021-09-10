#include "InboundMessage.h"

namespace UKControllerPlugin::Integration {
    /*
     * Get the data to go in the message.
     */
    auto InboundMessage::GetMessageData() const -> nlohmann::json
    {
        return this->data;
    }

    /*
     * Get the type of the message.
     */
    auto InboundMessage::GetMessageType() const -> MessageType
    {
        return this->type;
    }

    /*
     * Check that the basic message format is valid.
     */
    auto InboundMessage::MessageFormatValid(const nlohmann::json& message) -> bool
    {
        return
            message.is_object() &&
            message.contains("type") &&
            message.at("type").is_string() &&
            message.contains("version") &&
            message.at("version").is_number_integer() &&
            message.contains("id") &&
            message.at("id").is_string() &&
            message.contains("data") &&
            message.at("data").is_object();
    }

    /*
     * Given some JSON, make an inbound message
     */
    auto InboundMessage::FromJson(const nlohmann::json& json) -> std::shared_ptr<InboundMessage>
    {
        if (!MessageFormatValid(json)) {
            LogError("Invalid inbound message format from integration: " + json.dump());
            return nullptr;
        }

        return std::shared_ptr<InboundMessage>(
            new InboundMessage(
                json.at("id").get<std::string>(),
                MessageType{
                    json.at("type").get<std::string>(),
                    json.at("version").get<int>()
                },
                json.at("data")
            )
        );
    }
    
    auto InboundMessage::GetMessageId() const -> std::string
    {
        return this->id;
    }
} // namespace UKControllerPlugin::Integration
