#pragma once
#include "notifications/NotificationsRepository.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsignCollection.h"

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
                    const Api::ApiInterface& api,
                    const Controller::ActiveCallsignCollection& activeCallsigns
                );
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                void InitDialog(HWND hwnd, LPARAM lParam);
                void SelectNotification(HWND hwnd, NMLISTVIEW * details);
                void MarkNotificationAsRead(HWND hwnd) const;
                void OpenNotificationLink(HWND hwnd) const;
                LRESULT HighlightRelevantNotification(HWND hwnd, LPNMLVCUSTOMDRAW customDraw);
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
                auto GetSelectedNotification(HWND hwnd) const -> Notification *;
                auto GetSelectedItemFromNotificationsList(HWND hwnd) const -> LVITEM;
                auto GetNotificationsList(HWND hwnd) const -> HWND;

                // Repository containing notifications
                std::shared_ptr<NotificationsRepository> repository;

                // Api for marking notifications as read
                const Api::ApiInterface& api;

                // All the active callsigns
                const Controller::ActiveCallsignCollection& activeCallsigns;

                // Read status strings
                const std::wstring readString;
                const std::wstring unreadString;

                size_t selectedNotification = -1;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
