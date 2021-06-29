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
             * Get the data to go in the message.
             */
            virtual nlohmann::json GetMessageData() const = 0;

            /*
             * Get the type of the message.
             */
            virtual MessageType GetMessageType() const = 0;

            bool operator==(const MessageInterface& compare) const
            {
                return this->GetMessageType() == compare.GetMessageType() &&
                    this->GetMessageData() == compare.GetMessageData();
            }
    };
} // namespace UKControllerPlugin::Integration
