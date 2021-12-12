#pragma once

namespace UKControllerPlugin::Integration {
    class MessageInterface;

    /*
     * A class to be implemented in order to use
     * the IntegrationEventsFacade
     */
    class OutboundIntegrationEventHandler
    {
        public:
        virtual ~OutboundIntegrationEventHandler() = default;
        virtual void SendEvent(std::shared_ptr<MessageInterface> message) const = 0;
    };
} // namespace UKControllerPlugin::Integration
