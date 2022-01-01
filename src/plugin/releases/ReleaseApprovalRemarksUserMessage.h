#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::Releases {

    class ReleaseApprovalRemarksUserMessage : public Message::MessageSerializableInterface
    {
        public:
        ReleaseApprovalRemarksUserMessage(const std::string callsign, const std::string controller, const std::string remarks);
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
        const std::string controller;
        const std::string remarks;
    };
} // namespace UKControllerPlugin::Releases
