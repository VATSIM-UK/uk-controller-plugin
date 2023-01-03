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
            std::shared_ptr<const FirExitPointCollection> exitPoints,
            std::shared_ptr<const Integration::OutboundIntegrationEventHandler> outboundEvents);
        void IntentionCodeUpdated(const AircraftIntentionCode& intentionCode) override;

        private:
        // FIR exit points
        std::shared_ptr<const FirExitPointCollection> exitPoints;

        // Sends integration messages
        std::shared_ptr<const Integration::OutboundIntegrationEventHandler> outboundEvents;
    };

} // namespace UKControllerPlugin::IntentionCode
