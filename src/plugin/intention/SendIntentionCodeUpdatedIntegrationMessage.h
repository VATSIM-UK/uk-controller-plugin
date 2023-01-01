#pragma once
#include "IntentionCodeEventHandlerInterface.h"

namespace UKControllerPlugin::Integration {
    class OutboundIntegrationEventHandler;
} // namespace UKControllerPlugin::Integration

namespace UKControllerPlugin::IntentionCode {

    class FirExitPointCollection;

    /**
     * Class for sending intention code integration messages.
     */
    class SendIntentionCodeUpdatedIntegrationMessage : public IntentionCodeEventHandlerInterface
    {
        public:
        SendIntentionCodeUpdatedIntegrationMessage(
            const FirExitPointCollection& exitPoints, Integration::OutboundIntegrationEventHandler& outboundEvents);
        void IntentionCodeUpdated(const AircraftIntentionCode& intentionCode) override;

        private:
        // FIR exit points
        const FirExitPointCollection& exitPoints;

        // Sends integration messages
        Integration::OutboundIntegrationEventHandler& outboundEvents;
    };

} // namespace UKControllerPlugin::IntentionCode
