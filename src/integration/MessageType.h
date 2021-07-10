#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents the different types of message that can be passed.
     * Each message has a type, as well as a version.
     */
    using MessageType = struct MessageType
    {
        // The type of the message
        const std::string type;

        // What version of the message is being broadcast
        const int version;

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
    };
} // namespace UKControllerPlugin::Integration
