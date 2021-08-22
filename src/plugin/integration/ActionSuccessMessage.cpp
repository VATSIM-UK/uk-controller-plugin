#include "integration/ActionSuccessMessage.h"

namespace UKControllerPlugin::Integration {
    ActionSuccessMessage::ActionSuccessMessage(const std::string messageId)
        :messageId(messageId)
    { }
    
    std::string ActionSuccessMessage::GetMessageId() const
    {
        return this->messageId;
    }
    
    MessageType ActionSuccessMessage::GetMessageType() const
    {
        return {
            "action_success",
            1
        };
    }
} // namespace UKControllerPlugin::Integration
