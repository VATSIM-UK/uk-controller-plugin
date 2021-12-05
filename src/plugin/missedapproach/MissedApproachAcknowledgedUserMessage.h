#pragma once
#include <string>
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;
    
    class MissedApproachAcknowledgedUserMessage : public Message::MessageSerializableInterface
    {
        public:
        MissedApproachAcknowledgedUserMessage(const MissedApproach& approach);
        std::string MessageHandler(void) const override;
        std::string MessageSender(void) const override;
        std::string MessageString(void) const override;
        bool MessageShowHandler(void) const override;
        bool MessageMarkUnread(void) const override;
        bool MessageOverrideBusy(void) const override;
        bool MessageFlashHandler(void) const override;
        bool MessageRequiresConfirm(void) const override;
        
        private:
        
        const MissedApproach& approach;
    };
} // namespace UKControllerPlugin::Message
