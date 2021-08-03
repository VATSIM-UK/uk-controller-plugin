#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "notifications/NotificationsRepository.h"
#include "message/UserMessager.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
            Handles events in relation to notifications
        */
        class NotificationsEventHandler: public Controller::ActiveCallsignEventHandlerInterface
        {
            public:
                NotificationsEventHandler(
                    const std::shared_ptr<NotificationsRepository> notifications,
                    Message::UserMessager& messager
                );
                void ActiveCallsignAdded(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override {};
                void CallsignsFlushed() override {};

            private:

                // The repository of all the notifications
                const std::shared_ptr<NotificationsRepository> notifications;

                // Sends messages to the user
                Message::UserMessager& messager;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
