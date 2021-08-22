#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * A message that is sent to the integration when they have successfully
     * initialised with the UK Controller Plugin.
     */
    class InitialisationSuccessMessage : public MessageInterface
    {
        public:
            explicit InitialisationSuccessMessage(std::string messageId);
            ~InitialisationSuccessMessage() override = default;
            [[nodiscard]] MessageType GetMessageType() const override;
            [[nodiscard]] nlohmann::json GetMessageData() const override;
            [[nodiscard]] std::string GetMessageId() const override;
            
        private:
            
            // The id for the message to return
            std::string messageId;
    };
} // namespace UKControllerPlugin::Integration
