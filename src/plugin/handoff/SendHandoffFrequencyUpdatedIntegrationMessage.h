#pragma once
#include "DepartureHandoffResolvedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin::Integration {
    class OutboundIntegrationEventHandler;
} // namespace UKControllerPlugin::Integration

namespace UKControllerPlugin::Handoff {
    class SendHandoffFrequencyUpdatedIntegrationMessage
        : public UKControllerPluginUtils::EventHandler::EventHandler<DepartureHandoffResolvedEvent>
    {
        public:
        SendHandoffFrequencyUpdatedIntegrationMessage(Integration::OutboundIntegrationEventHandler& outboundEvent);
        void OnEvent(const DepartureHandoffResolvedEvent& event) override;

        private:
        Integration::OutboundIntegrationEventHandler& outboundEvent;
    };
} // namespace UKControllerPlugin::Handoff
