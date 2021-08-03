#pragma once
#include "integration/OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::Integration {
    class IntegrationClientManager;
    class MessageInterface;

    /*
     * A dummy class dedicated to handling outbound messages from the plugin
     * to integrations. This is used on duplicate plugins
     */
    class DummyOutboundIntegrationMessageHandler : public OutboundIntegrationEventHandler
    {
        public:
            void SendEvent(std::shared_ptr<MessageInterface> message) override;
    };
} // namespace UKControllerPlugin::Integration
