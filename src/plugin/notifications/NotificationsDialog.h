#pragma once
#include "api/ApiInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "notifications/NotificationsRepository.h"

namespace UKControllerPlugin::Notifications {
    /*
     *     Dialog for viewing all the notifications
     */
    class NotificationsDialog
    {
        public:
        NotificationsDialog(
            std::shared_ptr<NotificationsRepository> repository,
            const Api::ApiInterface& api,
            const Controller::ActiveCallsignCollection& activeCallsigns);
        static auto CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

        private:
        void InitDialog(HWND hwnd);
        void SelectNotification(HWND hwnd, NMLISTVIEW* details);
        void MarkNotificationAsRead(HWND hwnd) const;
        void OpenNotificationLink(HWND hwnd) const;
        auto HighlightRelevantNotification(LPNMLVCUSTOMDRAW customDraw) -> LRESULT;
        auto InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
        auto GetSelectedNotification(HWND hwnd) const -> Notification*;
        auto GetSelectedItemFromNotificationsList(HWND hwnd) const -> LVITEM;
        static auto GetNotificationsList(HWND hwnd) -> HWND;

        // Repository containing notifications
        std::shared_ptr<NotificationsRepository> repository;

        // Api for marking notifications as read
        const Api::ApiInterface& api;

        // All the active callsigns
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Read status strings
        const std::wstring readString;
        const std::wstring unreadString;

        const size_t noNotificationSelected = 99999999;
        size_t selectedNotification = noNotificationSelected;
    };
} // namespace UKControllerPlugin::Notifications
