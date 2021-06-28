#pragma once

namespace UKControllerPlugin::Integration {

    /*
     * Handles outbound messages from the plugin
     * to other sources via a given channel - e.g. window to window.
     */
    class OutboundMessageHandler
    {
        virtual void Send(std::string message, )
    };
} // namespace UKControllerPlugin::Integration
