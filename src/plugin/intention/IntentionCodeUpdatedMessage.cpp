#include "intention/IntentionCodeUpdatedMessage.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeUpdatedMessage::IntentionCodeUpdatedMessage(
        std::string callsign, std::string exitPoint, std::string code)
        : callsign(std::move(callsign)), code(std::move(code))
    {
        if (!exitPoint.empty()) {
            this->exitPoint = std::move(exitPoint);
        }
    }

    nlohmann::json IntentionCodeUpdatedMessage::GetMessageData() const
    {
        return nlohmann::json{
            {"callsign", this->callsign},
            {"exit_point", this->exitPoint},
            {"code", this->code},
        };
    }

    Integration::MessageType IntentionCodeUpdatedMessage::GetMessageType() const
    {
        return {"intention_code_updated", 1};
    }
} // namespace UKControllerPlugin::IntentionCode
