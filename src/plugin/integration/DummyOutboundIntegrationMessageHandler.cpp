#include "pch/pch.h"
#include "integration/DummyOutboundIntegrationMessageHandler.h"
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {
    void DummyOutboundIntegrationMessageHandler::SendEvent(std::shared_ptr<MessageInterface> message)
    {
        LogDebug("Skipping outbound integration event: " + message->ToJson().dump());
    }
} // namespace UKControllerPlugin::Integration
