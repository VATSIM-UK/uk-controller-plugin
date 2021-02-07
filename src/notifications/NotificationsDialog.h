#pragma once
#include "notifications/NotificationsRepository.h"
#include "api/ApiInterface.h"
#include "dialog/DialogManager.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
         *     Dialog for viewing all the notifications
        */
        class NotificationsDialog
        {
            public:

                NotificationsDialog(
                    std::shared_ptr<NotificationsRepository> repository,
                    const Api::ApiInterface& api
                );
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                void InitDialog(HWND hwnd, LPARAM lParam);
                void SelectNotification(HWND hwnd, NMLISTVIEW * details);
                void MarkNotificationAsRead(HWND hwnd) const;
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // Repository containing notifications
                std::shared_ptr<NotificationsRepository> repository;

                // Api for marking notifications as read
                const Api::ApiInterface& api;

                // Read status strings
                const std::wstring readString;
                const std::wstring unreadString;

                size_t selectedNotification = -1;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
