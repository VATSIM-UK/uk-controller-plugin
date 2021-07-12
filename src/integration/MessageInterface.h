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
             * Get the data to go in the message. Return the null
             * type if there is no data.
             */
            virtual nlohmann::json GetMessageData() const
            {
                return NONE;
            }

            /*
             * Get the type of the message.
             */
            virtual MessageType GetMessageType() const = 0;

            /*
             * Get the errors from the message. Return null type
             * if there are none.
             */
            virtual nlohmann::json GetErrorData() const
            {
                return NONE;
            }

            /*
             * Convert the message to JSON
             */
            nlohmann::json ToJson() const
            {
                nlohmann::json data = this->GetMessageType().ToJson();
                if (this->GetMessageData() != NONE) {
                    data["data"] = this->GetMessageData();
                }

                if (this->GetErrorData() != NONE) {
                    data["errors"] = this->GetErrorData();
                }

                return data;
            }

            bool operator==(const MessageInterface& compare) const
            {
                return this->GetMessageType() == compare.GetMessageType() &&
                    this->GetMessageData() == compare.GetMessageData();
            }

            inline static const nlohmann::json NONE = nlohmann::json::value_t::null;
    };
} // namespace UKControllerPlugin::Integration
