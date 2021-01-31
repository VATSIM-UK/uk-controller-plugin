#include "pch/stdafx.h"
#include "notifications/NotificationsModule.h"
#include "notifications/NotificationsRepositoryFactory.h"

namespace UKControllerPlugin {
    namespace Notifications {
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
        {
            NotificationsRepository repository = Make(container);
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
