#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::IntentionCode {

    /*
     * A message to let integrations know that a intention code has been updated
     */
    class IntentionCodeUpdatedMessage : public Integration::MessageInterface
    {
        public:
            IntentionCodeUpdatedMessage(
                std::string callsign,
                std::string exitPoint,
                std::string code
            );
            ~IntentionCodeUpdatedMessage() override = default;
            nlohmann::json GetMessageData() const override;
            Integration::MessageType GetMessageType() const override;

        private:
            // Who the codes for
            std::string callsign;

            // The exit point
            nlohmann::json exitPoint = nlohmann::json::value_t::null;

            // The code
            std::string code;
    };
} // namespace UKControllerPlugin::IntentionCode
