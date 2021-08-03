#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin::Integration {

    using messageversion_t = const int;
    using messagetype_t = const std::string;

    /*
     * Represents the different types of message that can be passed.
     * Each message has a type, as well as a version.
     */
    using MessageType = struct MessageType
    {
        // The type of the message
        messagetype_t type;

        // What version of the message is being broadcast
        messageversion_t version;

        nlohmann::json ToJson() const
        {
            return nlohmann::json::object({
                {"type", this->type},
                {"version", this->version}
            });
        }

        bool operator==(const MessageType& compare) const
        {
            return this->type == compare.type &&
                this->version == compare.version;
        }

        bool operator!=(const MessageType& compare) const
        {
            return !(*this == compare);
        }

        bool operator<(const MessageType& compare) const
        {
            return this->type < compare.type ||
                this->version < compare.version;
        }
    };
} // namespace UKControllerPlugin::Integration
