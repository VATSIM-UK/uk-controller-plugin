#include "notifications/UnreadNotificationsMessage.h"

namespace UKControllerPlugin {
    namespace Notifications {

        UnreadNotificationsMessage::UnreadNotificationsMessage(size_t numberUnread) : numberUnread(numberUnread)
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
                   " relevant to the position you are controlling. You can view " + "notifications from the OP menu.";
        }

        /*
            This is an alert message that we want users to be aware of as soon
            as they log into a position, so return true on all the interface methods
            to make it really obvious.
        */
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
    } // namespace Notifications
} // namespace UKControllerPlugin
