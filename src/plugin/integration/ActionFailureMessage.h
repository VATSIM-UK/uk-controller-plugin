#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {
    class ActionFailureMessage: public MessageInterface
    {
        public:
            ActionFailureMessage(std::string messageId, std::vector<std::string> errors);
            ~ActionFailureMessage() override = default;
            [[nodiscard]] std::vector<std::string> GetErrorData() const override;
            [[nodiscard]] std::string GetMessageId() const override;
            [[nodiscard]] MessageType GetMessageType() const override;
            
        private:
            const std::vector<std::string> errors;
            const std::string messageId;
    };
} // namespace UKControllerPlugin::Integration
