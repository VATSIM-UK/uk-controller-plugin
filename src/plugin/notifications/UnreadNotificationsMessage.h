#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
            A class representing a message to the user that they have unread
            notifications relevant to their controller position.
        */
        class UnreadNotificationsMessage: public Message::MessageSerializableInterface
        {
            public:
                explicit UnreadNotificationsMessage(size_t numberUnread);

                // Inherited via MessageSerializableInterface
                std::string MessageHandler(void) const override;
                std::string MessageSender(void) const override;
                std::string MessageString(void) const override;
                bool MessageShowHandler(void) const override;
                bool MessageMarkUnread(void) const override;
                bool MessageOverrideBusy(void) const override;
                bool MessageFlashHandler(void) const override;
                bool MessageRequiresConfirm(void) const override;

            private:
                const size_t numberUnread;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
