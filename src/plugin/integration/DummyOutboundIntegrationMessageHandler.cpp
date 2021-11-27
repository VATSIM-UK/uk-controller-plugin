#include "DummyOutboundIntegrationMessageHandler.h"
#include "MessageInterface.h"

namespace UKControllerPlugin::Integration {
    void DummyOutboundIntegrationMessageHandler::SendEvent(std::shared_ptr<MessageInterface> message) const
    {
        LogDebug("Skipping outbound integration event: " + message->ToJson().dump());
    }
} // namespace UKControllerPlugin::Integration
