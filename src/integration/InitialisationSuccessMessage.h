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
            ~InitialisationSuccessMessage() override = default;
            MessageType GetMessageType() const override;
            nlohmann::json GetMessageData() const override;
    };
} // namespace UKControllerPlugin::Integration
