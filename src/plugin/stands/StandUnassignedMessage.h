#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Stands {
    class StandUnassignedMessage : public Integration::MessageInterface
    {
        public:
        explicit StandUnassignedMessage(std::string callsign);
        ~StandUnassignedMessage() override = default;
        nlohmann::json GetMessageData() const override;
        Integration::MessageType GetMessageType() const override;

        private:
        // The callsign for the unassignment
        std::string callsign;
    };
} // namespace UKControllerPlugin::Stands
