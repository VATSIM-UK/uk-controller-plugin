#include "pch/pch.h"
#include "integration/ActionFailureMessage.h"

namespace UKControllerPlugin::Integration {
    ActionFailureMessage::ActionFailureMessage(std::string messageId, std::vector<std::string> errors)
        : errors(std::move(errors)), messageId(std::move(messageId))
    { }
    
    std::vector<std::string> UKControllerPlugin::Integration::ActionFailureMessage::GetErrorData() const
    {
        return this->errors;
    }
    
    std::string UKControllerPlugin::Integration::ActionFailureMessage::GetMessageId() const
    {
        return this->messageId;
    }
    
    MessageType UKControllerPlugin::Integration::ActionFailureMessage::GetMessageType() const
    {
        return {
            "action_failure",
            1
        };
    }
} // namespace UKControllerPlugin::Integration
