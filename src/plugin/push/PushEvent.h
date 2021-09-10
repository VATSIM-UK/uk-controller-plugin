#pragma once

namespace UKControllerPlugin::Push {
    /*
        Represents an event that is "pushed" from the API to the plugin, that is,
        an event that this plugin instance didn't generate.
    */
    using PushEvent = struct PushEvent
    {
        // The event associated with the message
        const std::string event;

        // The channel that the message came in from
        const std::string channel;

        // The data associated with the message
        const nlohmann::json data;

        // The raw message
        const std::string raw;

        auto operator==(const PushEvent& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Push
