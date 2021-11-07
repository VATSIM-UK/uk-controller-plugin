#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPlugin::Message {
    class UserMessager;
} // namespace UKControllerPlugin::Message

namespace UKControllerPlugin::Notifications {

    class NotificationsRepository;

    /*
        Handles events in relation to notifications
    */
    class NotificationsEventHandler : public Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        NotificationsEventHandler(
            std::shared_ptr<NotificationsRepository> notifications, Message::UserMessager& messager);
        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override{};

        private:
        // The repository of all the notifications
        const std::shared_ptr<NotificationsRepository> notifications;

        // Sends messages to the user
        Message::UserMessager& messager;
    };
} // namespace UKControllerPlugin::Notifications
