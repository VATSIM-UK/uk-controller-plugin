#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachTriggeredMessage : public Integration::MessageInterface
    {
        public:
        MissedApproachTriggeredMessage(
            std::string callsign, bool triggeredByUser, std::chrono::system_clock::time_point expiresAt);
        auto GetMessageData() const -> nlohmann::json override;
        auto GetMessageType() const -> Integration::MessageType override;

        private:
        std::string callsign;
        bool triggeredByUser;
        std::chrono::system_clock::time_point expiresAt;
    };
} // namespace UKControllerPlugin::MissedApproach
