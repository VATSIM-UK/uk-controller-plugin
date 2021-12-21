#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;

    class MissedApproachAcknowledgedUserMessage : public Message::MessageSerializableInterface
    {
        public:
        MissedApproachAcknowledgedUserMessage(const MissedApproach& approach);
        auto MessageHandler() const -> std::string override;
        auto MessageSender() const -> std::string override;
        auto MessageString() const -> std::string override;
        auto MessageShowHandler() const -> bool override;
        auto MessageMarkUnread() const -> bool override;
        auto MessageOverrideBusy() const -> bool override;
        auto MessageFlashHandler() const -> bool override;
        auto MessageRequiresConfirm() const -> bool override;

        private:
        const MissedApproach& approach;
    };
} // namespace UKControllerPlugin::MissedApproach
