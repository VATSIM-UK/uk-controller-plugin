#include "NotificationsEventHandler.h"
#include "NotificationsRepository.h"
#include "UnreadNotificationsMessage.h"
#include "controller/ActiveCallsign.h"
#include "message/UserMessager.h"

namespace UKControllerPlugin::Notifications {

    NotificationsEventHandler::NotificationsEventHandler(
        std::shared_ptr<NotificationsRepository> notifications, Message::UserMessager& messager)
        : notifications(std::move(notifications)), messager(messager)
    {
    }

    /*
     * If the user has connected with a callsign
     */
    void NotificationsEventHandler::ActiveCallsignAdded(const Controller::ActiveCallsign& callsign, bool userCallsign)
    {
        if (!userCallsign) {
            return;
        }

        size_t concernedNotifications = std::count_if(
            this->notifications->cbegin(),
            this->notifications->cend(),
            [callsign](const std::unique_ptr<Notification>& notification) -> bool {
                return !notification->IsRead() && notification->IsRelevant(callsign.GetNormalisedPosition());
            });

        if (concernedNotifications == 0) {
            return;
        }

        UnreadNotificationsMessage message(concernedNotifications);
        this->messager.SendMessageToUser(message);
    }
} // namespace UKControllerPlugin::Notifications
