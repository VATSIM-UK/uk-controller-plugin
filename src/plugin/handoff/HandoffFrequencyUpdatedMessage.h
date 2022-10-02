#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Handoff {

    /*
     * A message to let integrations know that a handoff frequency has been updated
     */
    class HandoffFrequencyUpdatedMessage : public Integration::MessageInterface
    {
        public:
        HandoffFrequencyUpdatedMessage(std::string callsign, std::string frequency);
        ~HandoffFrequencyUpdatedMessage() override = default;
        nlohmann::json GetMessageData() const override;
        Integration::MessageType GetMessageType() const override;

        private:
        // Who the codes for
        std::string callsign;

        // The code
        std::string frequency;
    };
} // namespace UKControllerPlugin::Handoff
