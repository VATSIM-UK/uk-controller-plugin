#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Push {
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

            bool operator==(const PushEvent& compare) const
            {
                return this->channel == compare.channel &&
                    this->event == compare.event &&
                    this->data == compare.data;
            }
        };
    } // namespace Push
}  // namespace UKControllerPlugin
