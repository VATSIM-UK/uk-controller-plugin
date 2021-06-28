#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin::Integration {

    /*
     * Represents a message that's outbound from the plugin to other
     * interested plugins and programs.
     */
    class OutboundMessageInterface
    {
        public:
            virtual ~OutboundMessageInterface() = default;

            /*
             * Get the data to go in the message.
             */
            virtual nlohmann::json GetMessageData() const = 0;

            /*
             * Get the version of the message.
             */
            virtual unsigned int GetMessageVersion() const = 0;

            /*
             * Get the type of the message.
             */
            virtual std::string GetMessageType() const = 0;
    };
} // namespace UKControllerPlugin::Integration
