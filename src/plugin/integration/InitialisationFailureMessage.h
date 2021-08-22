#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * A message that is sent to the integration when they have successfully
     * initialised with the UK Controller Plugin.
     */
    class InitialisationFailureMessage : public MessageInterface
    {
        public:
            InitialisationFailureMessage(std::string messageId, std::vector<std::string> errors);
            ~InitialisationFailureMessage() override = default;
            [[nodiscard]] std::string GetMessageId() const override;
            [[nodiscard]] MessageType GetMessageType() const override;
            [[nodiscard]] std::vector<std::string> GetErrorData() const override;

        private:
            // Message id
            std::string messageId;
            
            // The errors
            const std::vector<std::string> errors;
    };
} // namespace UKControllerPlugin::Integration
