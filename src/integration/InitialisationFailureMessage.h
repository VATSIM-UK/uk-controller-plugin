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
            explicit InitialisationFailureMessage(std::vector<std::string> errors);
            ~InitialisationFailureMessage() override = default;
            MessageType GetMessageType() const override;
            nlohmann::json GetErrorData() const override;

        private:
            // The errors
            const std::vector<std::string> errors;
    };
} // namespace UKControllerPlugin::Integration
