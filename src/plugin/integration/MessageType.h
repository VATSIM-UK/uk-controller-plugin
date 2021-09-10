#pragma once

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

        [[nodiscard]] auto ToJson() const -> nlohmann::json;
        [[nodiscard]] auto operator==(const MessageType& compare) const -> bool;
        [[nodiscard]] auto operator!=(const MessageType& compare) const -> bool;
        [[nodiscard]] auto operator<(const MessageType& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Integration
