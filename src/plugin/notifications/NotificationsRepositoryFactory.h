#pragma once
#include "notifications/NotificationsRepository.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPositionHierarchyFactory.h"

namespace UKControllerPlugin {
    namespace Notifications {
        std::shared_ptr<NotificationsRepository> Make(const Bootstrap::PersistenceContainer& container);
        void ProcessNotifications(
            NotificationsRepository& repository,
            const nlohmann::json& notifications,
            const Controller::ControllerPositionHierarchyFactory& hierarchyFactory);
        void ProcessUnreadNotifications(NotificationsRepository& repository, const nlohmann::json& notifications);
        bool NotificationsValid(
            const nlohmann::json& notifications,
            const Controller::ControllerPositionHierarchyFactory& hierarchyFactory);
        bool NotificationValid(
            const nlohmann::json& notification, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory);
        bool ControllersValid(
            const nlohmann::json& controllers, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory);
        bool UnreadNotificationsValid(const nlohmann::json& notifications);
    } // namespace Notifications
} // namespace UKControllerPlugin
