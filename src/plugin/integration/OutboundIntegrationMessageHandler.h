#pragma once
#include "OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::Integration {
    class IntegrationClientManager;
    class MessageInterface;

    /*
     * A class dedicated to handling outbound messages from the plugin
     * to integrations.
     */
    class OutboundIntegrationMessageHandler : public OutboundIntegrationEventHandler
    {
        public:
        explicit OutboundIntegrationMessageHandler(std::shared_ptr<IntegrationClientManager> clientManager);
        void SendEvent(std::shared_ptr<MessageInterface> message) const override;

        private:
        const std::shared_ptr<IntegrationClientManager> clientManager;

        // Array to ensure we only log the same message type once
        mutable std::set<std::string> apiLoggedTypes;
    };
} // namespace UKControllerPlugin::Integration
