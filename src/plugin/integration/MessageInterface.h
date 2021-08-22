#pragma once
#include "pch/pch.h"
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
             * Get the data to go in the message, which is merged
             * into the message under the "data" key.
             */
            [[nodiscard]] virtual nlohmann::json GetMessageData() const
            {
                return nlohmann::json::object();
            }

            /*
             * Get the errors associated with the message, which is merged
             * in under the "errors" key.
             */
            [[nodiscard]] virtual std::vector<std::string> GetErrorData() const
            {
                return {};
            }
            
            /*
             * Gets a unique id associated with the message so that
             * things can tied together at each end.
             */
            [[nodiscard]] virtual std::string GetMessageId() const
            {
                return "";
            }

            /*
             * Get the type of the message.
             */
            [[nodiscard]] virtual MessageType GetMessageType() const = 0;

            /*
             * Convert the message to JSON
             */
            [[nodiscard]] nlohmann::json ToJson() const
            {
                nlohmann::json baseMessage = this->GetMessageType().ToJson();
                auto data = this->GetMessageData();
                if (!data.empty()) {
                    baseMessage["data"] = data;
                }

                auto errors = this->GetErrorData();
                if (!errors.empty()) {
                    baseMessage["errors"] = errors;
                }
                
                if (!this->GetMessageId().empty()) {
                    baseMessage["id"] = this->GetMessageId();
                }

                return baseMessage;
            }

            bool operator==(const MessageInterface& compare) const
            {
                return this->ToJson() == compare.ToJson();
            }
    };
} // namespace UKControllerPlugin::Integration
