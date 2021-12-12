#include "NotificationsRepositoryFactory.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "time/ParseTimeStrings.h"

using UKControllerPlugin::Time::invalidTime;
using UKControllerPlugin::Time::ParseTimeString;

namespace UKControllerPlugin::Notifications {

    auto Make(const Bootstrap::PersistenceContainer& container) -> std::shared_ptr<NotificationsRepository>
    {
        std::shared_ptr<NotificationsRepository> repository = std::make_unique<NotificationsRepository>();

        nlohmann::json allNotifications;
        try {
            allNotifications = container.api->GetAllNotifications();
        } catch (Api::ApiException& apiException) {
            LogError("Failed to get notifications " + std::string(apiException.what()));
            return repository;
        }

        if (!NotificationsValid(allNotifications, *container.controllerHierarchyFactory)) {
            LogError("API returned invalid notifications");
            return repository;
        }

        ProcessNotifications(*repository, allNotifications, *container.controllerHierarchyFactory);
        LogInfo("Loaded " + std::to_string(repository->Count()) + " notifications");

        nlohmann::json unreadNotifications;
        try {
            unreadNotifications = container.api->GetUnreadNotifications();
        } catch (Api::ApiException& apiException) {
            LogError("Failed to get unread notifications " + std::string(apiException.what()));
            return repository;
        }

        if (!UnreadNotificationsValid(unreadNotifications)) {
            LogError("API returned invalid unread notifications");
            return repository;
        }

        ProcessUnreadNotifications(*repository, unreadNotifications);
        LogInfo("There are " + std::to_string(repository->CountUnread()) + " unread notifications");
        return repository;
    }

    void ProcessNotifications(
        NotificationsRepository& repository,
        const nlohmann::json& notifications,
        const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
    {
        for (auto notification = notifications.cbegin(); notification != notifications.cend(); ++notification) {
            repository.Add(std::make_unique<Notification>(
                notification->at("id").get<int>(),
                notification->at("title").get<std::string>(),
                notification->at("body").get<std::string>(),
                ParseTimeString(notification->at("valid_from")),
                ParseTimeString(notification->at("valid_to")),
                hierarchyFactory.CreateFromJsonByCallsign(notification->at("controllers")),
                notification->at("link").is_null() ? "" : notification->at("link").get<std::string>()));
        }
    }

    void ProcessUnreadNotifications(NotificationsRepository& repository, const nlohmann::json& notifications)
    {
        std::set<int> unreadNotifications;
        for (auto unreadNotification = notifications.cbegin(); unreadNotification != notifications.cend();
             ++unreadNotification) {
            unreadNotifications.insert(unreadNotification->at("id").get<int>());
        }

        for (auto notification = repository.cbegin(); notification != repository.cend(); ++notification) {
            auto unreadNotification = unreadNotifications.find((*notification)->Id());
            if (unreadNotification == unreadNotifications.cend()) {
                (*notification)->Read();
            }
        }
    }

    auto NotificationsValid(
        const nlohmann::json& notifications, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        return notifications.is_array() && std::find_if_not(
                                               notifications.cbegin(),
                                               notifications.cend(),
                                               [hierarchyFactory](const nlohmann::json& notification) -> bool {
                                                   return NotificationValid(notification, hierarchyFactory);
                                               }) == notifications.cend();
    }

    auto NotificationValid(
        const nlohmann::json& notification, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        return notification.is_object() && notification.contains("id") && notification.at("id").is_number_integer() &&
               notification.contains("title") && notification.at("title").is_string() &&
               notification.contains("body") && notification.at("body").is_string() &&
               notification.contains("valid_from") && notification.at("valid_from").is_string() &&
               ParseTimeString(notification.at("valid_from").get<std::string>()) != invalidTime &&
               notification.contains("valid_to") && notification.at("valid_to").is_string() &&
               ParseTimeString(notification.at("valid_to").get<std::string>()) != invalidTime &&
               notification.contains("controllers") &&
               ControllersValid(notification.at("controllers"), hierarchyFactory) && notification.contains("link") &&
               (notification.at("link").is_string() || notification.at("link").is_null());
    }

    auto ControllersValid(
        const nlohmann::json& controllers, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        try {
            static_cast<void>(hierarchyFactory.CreateFromJsonByCallsign(controllers));
            return true;
        } catch (...) {
            return false;
        }
    }

    auto UnreadNotificationsValid(const nlohmann::json& notifications) -> bool
    {
        return notifications.is_array() &&
               std::find_if_not(notifications.cbegin(), notifications.cend(), [](const nlohmann::json& notification) {
                   return notification.is_object() && notification.contains("id") &&
                          notification.at("id").is_number_integer();
               }) == notifications.cend();
    }
} // namespace UKControllerPlugin::Notifications
