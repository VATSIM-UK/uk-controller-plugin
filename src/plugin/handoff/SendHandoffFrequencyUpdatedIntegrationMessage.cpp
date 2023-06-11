#include "HandoffFrequencyUpdatedMessage.h"
#include "ResolvedHandoff.h"
#include "SendHandoffFrequencyUpdatedIntegrationMessage.h"
#include "controller/ControllerPosition.h"
#include "datablock/DatablockFunctions.h"
#include "integration/OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin::Handoff {

    SendHandoffFrequencyUpdatedIntegrationMessage::SendHandoffFrequencyUpdatedIntegrationMessage(
        Integration::OutboundIntegrationEventHandler& outboundEvent)
        : outboundEvent(outboundEvent)
    {
    }

    void SendHandoffFrequencyUpdatedIntegrationMessage::OnEvent(const DepartureHandoffResolvedEvent& event)
    {
        this->outboundEvent.SendEvent(std::make_shared<HandoffFrequencyUpdatedMessage>(
            event.handoff->callsign,
            Datablock::FrequencyStringFromDouble(event.handoff->resolvedController->GetFrequency())));
    }
} // namespace UKControllerPlugin::Handoff
