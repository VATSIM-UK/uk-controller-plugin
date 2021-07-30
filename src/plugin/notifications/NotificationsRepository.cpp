#include "pch/pch.h"
#include "notifications/NotificationsRepository.h"

namespace UKControllerPlugin {
    namespace Notifications {
        void NotificationsRepository::Add(std::unique_ptr<Notification> notification)
        {
            this->notifications.insert(std::move(notification));
        }

        Notification* const NotificationsRepository::Get(int id) const
        {
            auto notification = std::find_if(
                this->notifications.cbegin(),
                this->notifications.cend(),
                [id](const std::unique_ptr<Notification>& notification) -> bool
                {
                    return notification->Id() == id;
                }
            );

            return notification == this->notifications.cend() ? nullptr : notification->get();
        }

        size_t NotificationsRepository::Count() const
        {
            return this->notifications.size();
        }

        /*
         * Returns the number of unread notifications
         */
        int NotificationsRepository::CountUnread() const
        {
            return std::count_if(
                this->notifications.cbegin(),
                this->notifications.cend(),
                [](const std::unique_ptr<Notification> & notification) -> bool
                {
                    return !notification->IsRead();
                }
            );
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
