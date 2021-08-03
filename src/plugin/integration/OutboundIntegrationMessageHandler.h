#pragma once
#include "integration/OutboundIntegrationEventHandler.h"

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
            void SendEvent(std::shared_ptr<MessageInterface> message) override;

        private:
            const std::shared_ptr<IntegrationClientManager> clientManager;
    };
} // namespace UKControllerPlugin::Integration
