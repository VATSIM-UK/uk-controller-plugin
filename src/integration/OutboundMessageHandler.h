#pragma once

namespace UKControllerPlugin::Integration {

    class MessageInterface;

    /*
     * Handles outbound messages from the plugin
     * to other sources via a given channel - e.g. window to window.
     */
    class OutboundMessageHandler
    {
        public:
            virtual ~OutboundMessageHandler() = default;
            virtual void Send(std::shared_ptr<MessageInterface> message) = 0;
    };
} // namespace UKControllerPlugin::Integration
