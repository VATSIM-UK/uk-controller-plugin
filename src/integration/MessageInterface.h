#pragma once
#include "pch/stdafx.h"
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents a message that's outbound from the plugin to other
     * interested plugins and programs.
     */
    class MessageInterface
    {
        public:
            virtual ~MessageInterface() = default;

            /*
             * Get the data to go in the message, this gets merged
             * with the message type information to make a complete
             * message.
             */
            virtual nlohmann::json GetMessageData() const
            {
                return nlohmann::json::object();
            }

            /*
             * Get the type of the message.
             */
            virtual MessageType GetMessageType() const = 0;

            /*
             * Convert the message to JSON
             */
            nlohmann::json ToJson() const
            {
                nlohmann::json data = this->GetMessageData();
                data.update(this->GetMessageType().ToJson());
                return data;
            }

            bool operator==(const MessageInterface& compare) const
            {
                return this->ToJson() == compare.ToJson();
            }
    };
} // namespace UKControllerPlugin::Integration
