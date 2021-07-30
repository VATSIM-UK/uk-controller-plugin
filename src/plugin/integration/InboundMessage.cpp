#include "pch/pch.h"
#include "integration/InboundMessage.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {
    /*
     * Get the data to go in the message.
     */
    nlohmann::json InboundMessage::GetMessageData() const
    {
        return this->data;
    }

    /*
     * Get the type of the message.
     */
    MessageType InboundMessage::GetMessageType() const
    {
        return this->type;
    }

    /*
     * Check that the basic message format is valid.
     */
    bool InboundMessage::MessageFormatValid(const nlohmann::json& message)
    {
        return
            message.is_object() &&
            message.contains("type") &&
            message.at("type").is_string() &&
            message.contains("version") &&
            message.at("version").is_number_integer() &&
            message.contains("data") &&
            message.at("data").is_object();
    }

    /*
     * Given some JSON, make an inbound message
     */
    std::shared_ptr<InboundMessage> InboundMessage::FromJson(const nlohmann::json& json)
    {
        if (!MessageFormatValid(json)) {
            LogError("Invalid inbound message format from integration: " + json.dump());
            return nullptr;
        }

        return std::shared_ptr<InboundMessage>(
            new InboundMessage(
                MessageType{
                    json.at("type").get<std::string>(),
                    json.at("version").get<int>()
                },
                json.at("data")
            )
        );
    }
} // namespace UKControllerPlugin::Integration
