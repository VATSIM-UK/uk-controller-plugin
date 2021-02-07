#include "pch/stdafx.h"
#include "notifications/UnreadNotificationsMessage.h"

namespace UKControllerPlugin {
    namespace Notifications {

        UnreadNotificationsMessage::UnreadNotificationsMessage(size_t numberUnread)
            : numberUnread(numberUnread)
        {
        }

        std::string UnreadNotificationsMessage::MessageHandler(void) const
        {
            return "UKCP_NOTIFICATION";
        }

        std::string UnreadNotificationsMessage::MessageSender(void) const
        {
            return "UKCP";
        }

        std::string UnreadNotificationsMessage::MessageString(void) const
        {
            std::string inflection = numberUnread == 1 ? "notification" : "notifications";
            return "You have " + std::to_string(this->numberUnread) + " unread " + inflection +
                " relevant to the position you are controlling.";
        }

        bool UnreadNotificationsMessage::MessageShowHandler(void) const
        {
            return true;
        }

        bool UnreadNotificationsMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        bool UnreadNotificationsMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        bool UnreadNotificationsMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        bool UnreadNotificationsMessage::MessageRequiresConfirm(void) const
        {
            return true;
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
