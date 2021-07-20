#pragma once

namespace UKControllerPlugin::Integration {
    class IntegrationClientManager;
    class MessageInterface;

    /*
     * A class dedicated to handling outbound messages from the plugin
     * to integrations.
     */
    class OutboundIntegrationMessageHandler
    {
        public:
            OutboundIntegrationMessageHandler(std::shared_ptr<IntegrationClientManager> clientManager);
            void SendOutboundMessage(std::shared_ptr<MessageInterface> message);
            void SendOutboundMessage(std::shared_ptr<MessageInterface> message, int clientId) const;

        private:
            const std::shared_ptr<IntegrationClientManager> clientManager;
    };
} // namespace UKControllerPlugin::Integration
