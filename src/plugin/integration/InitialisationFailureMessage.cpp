#include "integration/InitialisationFailureMessage.h"

namespace UKControllerPlugin::Integration {

    InitialisationFailureMessage::InitialisationFailureMessage(std::string messageId, std::vector<std::string> errors)
        : messageId(std::move(messageId)), errors(std::move(errors))
    {
    }

    MessageType InitialisationFailureMessage::GetMessageType() const
    {
        return {"initialisation_failure", 1};
    }

    std::vector<std::string> InitialisationFailureMessage::GetErrorData() const
    {
        return this->errors;
    }

    std::string InitialisationFailureMessage::GetMessageId() const
    {
        return this->messageId;
    }
} // namespace UKControllerPlugin::Integration
