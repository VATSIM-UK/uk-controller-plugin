#include "AircraftIntentionCode.h"
#include "FirExitPoint.h"
#include "FirExitPointCollection.h"
#include "IntentionCodeMetadata.h"
#include "IntentionCodeModel.h"
#include "IntentionCodeUpdatedMessage.h"
#include "SendIntentionCodeUpdatedIntegrationMessage.h"
#include "integration/OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::IntentionCode {

    SendIntentionCodeUpdatedIntegrationMessage::SendIntentionCodeUpdatedIntegrationMessage(
        const FirExitPointCollection& exitPoints, Integration::OutboundIntegrationEventHandler& outboundEvents)
        : exitPoints(exitPoints), outboundEvents(outboundEvents)
    {
    }

    void SendIntentionCodeUpdatedIntegrationMessage::IntentionCodeUpdated(const AircraftIntentionCode& intentionCode)
    {
        const auto exitPoint = intentionCode.matchedIntentionCode
                                   ? exitPoints.PointById(intentionCode.matchedIntentionCode->Metadata().exitPoint)
                                   : nullptr;

        outboundEvents.SendEvent(std::make_shared<IntentionCodeUpdatedMessage>(
            intentionCode.callsign, exitPoint ? exitPoint->Identifier() : "", intentionCode.intentionCode));
    }
} // namespace UKControllerPlugin::IntentionCode
