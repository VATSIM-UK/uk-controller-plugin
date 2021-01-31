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
                    Controller::ControllerPositionHierarchy controllers
                );

                int Id() const;
                std::string Title() const;
                std::string Body() const;
                bool Active() const;
                bool IsRelevant(const Controller::ControllerPosition & position) const;
                bool IsRead() const;
                bool Read();

            private:

                // The id of the notification on the API
                int id;

                // The title of the notification
                std::string title;

                // The body of the notification
                std::string body;

                // When the notification is valid from
                std::chrono::system_clock::time_point validFrom;

                // When the notification is valid to
                std::chrono::system_clock::time_point validTo;

                // The controllers the notification applies to
                Controller::ControllerPositionHierarchy controllers;

                // Whether or not the user has read the notification
                bool read = false;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
