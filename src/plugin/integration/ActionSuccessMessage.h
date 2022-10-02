#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {
    class ActionSuccessMessage : public MessageInterface
    {
        public:
        explicit ActionSuccessMessage(std::string messageid);
        ~ActionSuccessMessage() override = default;
        [[nodiscard]] std::string GetMessageId() const override;
        [[nodiscard]] MessageType GetMessageType() const override;

        private:
        const std::string messageId;
    };
} // namespace UKControllerPlugin::Integration
