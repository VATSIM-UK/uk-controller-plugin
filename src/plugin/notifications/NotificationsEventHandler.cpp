#include "pch/stdafx.h"
#include "notifications/NotificationsEventHandler.h"
#include "notifications/UnreadNotificationsMessage.h"

namespace UKControllerPlugin {
    namespace Notifications {

        NotificationsEventHandler::NotificationsEventHandler(
            const std::shared_ptr<NotificationsRepository> notifications,
            Message::UserMessager& messager
        ) : notifications(notifications), messager(messager)
        {
        }

        /*
         * If the user has connected with a callsign
         */
        void NotificationsEventHandler::ActiveCallsignAdded(
            const Controller::ActiveCallsign& callsign,
            bool userCallsign
        )
        {
            if (!userCallsign)
            {
                return;
            }

            size_t concernedNotifications = std::count_if(
                this->notifications->cbegin(),
                this->notifications->cend(),
                [callsign](const std::unique_ptr<Notification> & notification) -> bool
                {
                    return !notification->IsRead() && notification->IsRelevant(callsign.GetNormalisedPosition());
                }
            );

            if (concernedNotifications == 0)
            {
                return;
            }

            UnreadNotificationsMessage message(concernedNotifications);
            this->messager.SendMessageToUser(message);
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
