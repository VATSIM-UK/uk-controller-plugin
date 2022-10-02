#include "integration/InitialisationSuccessMessage.h"
#include "update/PluginVersion.h"

namespace UKControllerPlugin::Integration {

    InitialisationSuccessMessage::InitialisationSuccessMessage(std::string messageId) : messageId(std::move(messageId))
    {
    }

    nlohmann::json InitialisationSuccessMessage::GetMessageData() const
    {
        return {{"ukcp_version", Plugin::PluginVersion::version}};
    }

    MessageType InitialisationSuccessMessage::GetMessageType() const
    {
        return {"initialisation_success", 1};
    }

    std::string InitialisationSuccessMessage::GetMessageId() const
    {
        return this->messageId;
    }
} // namespace UKControllerPlugin::Integration
