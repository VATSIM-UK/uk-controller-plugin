#include "pch/pch.h"
#include "integration/InitialisationFailureMessage.h"

namespace UKControllerPlugin::Integration {

    InitialisationFailureMessage::InitialisationFailureMessage(std::vector<std::string> errors): errors(
        std::move(errors)) {}

    MessageType InitialisationFailureMessage::GetMessageType() const
    {
        return {
            "initialisation_failure",
            1
        };
    }

    nlohmann::json InitialisationFailureMessage::GetErrorData() const
    {
        return this->errors;
    }
} // namespace UKControllerPlugin::Integration
