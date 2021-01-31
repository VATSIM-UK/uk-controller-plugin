#pragma once
#include "notifications/Notification.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
            A new class
        */
        class NotificationsRepository
        {
            public:
                NotificationsRepository() = default;
                void Add(std::unique_ptr<Notification> notification);
                Notification* const Get(int id) const;
                size_t Count() const;

                // Public type definitions for a custom iterator over the class.
                typedef std::set<std::unique_ptr<Notification>> AllNotifications;
                typedef AllNotifications::iterator iterator;
                typedef AllNotifications::const_iterator const_iterator;
                const_iterator cbegin() const { return notifications.cbegin(); }
                const_iterator cend() const { return notifications.cend(); }

            private:

                // All notifications
                std::set<std::unique_ptr<Notification>> notifications;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
