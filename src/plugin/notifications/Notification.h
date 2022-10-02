#pragma once
#include "controller/ControllerPositionHierarchy.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
            Represents a notification for a controller
        */
        class Notification
        {
            public:
            Notification(
                int id,
                std::string title,
                std::string body,
                std::chrono::system_clock::time_point validFrom,
                std::chrono::system_clock::time_point validTo,
                std::unique_ptr<Controller::ControllerPositionHierarchy> controllers,
                std::string link);

            int Id() const;
            std::string Title() const;
            std::string Body() const;
            std::string Link() const;
            bool Active() const;
            bool IsRelevant(const Controller::ControllerPosition& position) const;
            bool IsRead() const;
            void Read();

            private:
            // The id of the notification on the API
            const int id;

            // The title of the notification
            const std::string title;

            // The body of the notification
            const std::string body;

            // The read more link for the notification
            const std::string link;

            // When the notification is valid from
            const std::chrono::system_clock::time_point validFrom;

            // When the notification is valid to
            const std::chrono::system_clock::time_point validTo;

            // The controllers the notification applies to
            const std::unique_ptr<Controller::ControllerPositionHierarchy> controllers;

            // Whether or not the user has read the notification
            bool read = false;
        };
    } // namespace Notifications
} // namespace UKControllerPlugin
