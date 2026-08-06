#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Releases {

    class ReleasePendingReminderUserMessage : public Message::MessageSerializableInterface
    {
        public:
        ReleasePendingReminderUserMessage(std::string callsign, std::string requestingController);
        auto MessageHandler() const -> std::string override;
        auto MessageSender() const -> std::string override;
        auto MessageString() const -> std::string override;
        auto MessageShowHandler() const -> bool override;
        auto MessageMarkUnread() const -> bool override;
        auto MessageOverrideBusy() const -> bool override;
        auto MessageFlashHandler() const -> bool override;
        auto MessageRequiresConfirm() const -> bool override;

        private:
        const std::string callsign;
        const std::string requestingController;
    };
} // namespace UKControllerPlugin::Releases
