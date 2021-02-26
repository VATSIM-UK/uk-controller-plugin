#include "pch/stdafx.h"
#include "notifications/NotificationsDialog.h"
#include "api/ApiException.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin {
    namespace Notifications {


        NotificationsDialog::NotificationsDialog(
            std::shared_ptr<NotificationsRepository> repository,
            const Api::ApiInterface& api,
            const Controller::ActiveCallsignCollection& activeCallsigns
        ) : repository(std::move(repository)), api(api), activeCallsigns(activeCallsigns), readString(L"Read"),
            unreadString(L"Unread")
        {
        }


        /*
            Private dialog procedure for the dialog, should be used
            against a bound instance.
        */
        LRESULT NotificationsDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd, lParam);
                    return TRUE;
                };
                // Window closed
                case WM_CLOSE: {
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDOK: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_READ_NOTIFICATION: {
                            this->MarkNotificationAsRead(hwnd);
                            return TRUE;
                        }
                        default:
                            return FALSE;
                    }
                }
                case WM_NOTIFY: {
                    switch (((LPNMHDR)lParam)->code) {
                        case LVN_ITEMCHANGED: {
                            if (((LPNMHDR)lParam)->idFrom == IDC_NOTIFICATIONS_LIST){
                                this->SelectNotification(hwnd, reinterpret_cast<NMLISTVIEW*>((LPNMHDR)lParam));
                                return TRUE;
                            }
                            return FALSE;
                        }
                        case NM_CLICK: {
                            if(wParam == IDC_NOTIFICATION_LINK) {
                                NMLINK* link = reinterpret_cast<NMLINK*>(lParam);
                                ShellExecute(NULL, L"open", link->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
                                return TRUE;
                            }
                            return FALSE;
                        }
                        case NM_CUSTOMDRAW: {
                            LPNMLVCUSTOMDRAW customDraw = (LPNMLVCUSTOMDRAW)lParam;
                            if (((LPNMHDR)lParam)->idFrom == IDC_NOTIFICATIONS_LIST) {
                                switch (customDraw->nmcd.dwDrawStage) {
                                    case CDDS_PREPAINT:
                                        SetWindowLongPtr(hwnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
                                        return CDRF_NOTIFYITEMDRAW;
                                    case CDDS_ITEMPREPAINT:
                                        return this->HighlightRelevantNotification(hwnd, customDraw);
                                    default:
                                        return FALSE;
                                }
                            }

                            return FALSE;
                        }
                        default:
                            return FALSE;
                    }
                }
            }
            return FALSE;
        }

        /*
            Public dialog procedure for the dialog
        */
        LRESULT NotificationsDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Notifications dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<Dialog::DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Notifications dialog closed");
            }

            NotificationsDialog * dialog = reinterpret_cast<NotificationsDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
         * Set up the notifications list
         */
        void NotificationsDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            HWND notificationsList = GetDlgItem(hwnd, IDC_NOTIFICATIONS_LIST);

            // Make items highlight on row select
            SendMessage(notificationsList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT & ~WS_HSCROLL);

            // Create the title column
            LVCOLUMN titleColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                250,
                L"Title",
                0
            };
            ListView_InsertColumn(notificationsList, 0, &titleColumn);

            // Create the read column
            LVCOLUMN readColumn = {
                LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                LVCFMT_LEFT | LVCFMT_FIXED_WIDTH,
                100,
                L"Read",
                1
            };
            ListView_InsertColumn(notificationsList, 1, &readColumn);

            // Add all the notifications
            int itemNumber = 0;
            std::for_each(
                this->repository->cbegin(),
                this->repository->cend(),
                [&itemNumber, &notificationsList, this](const std::unique_ptr<Notification> & notification) {
                    LVITEM item;
                    item.mask = LVIF_TEXT | LVIF_PARAM;
                    item.iItem = itemNumber;
                    item.iSubItem = 0;
                    item.lParam = (LPARAM) notification.get();

                    // The title of the notification
                    std::wstring title = HelperFunctions::ConvertToWideString(notification->Title());
                    item.pszText = (LPWSTR) title.c_str();
                    ListView_InsertItem(notificationsList, &item);

                    // Whether or not the notification has been read
                    std::wstring readText = notification->IsRead() ? this->readString : this->unreadString;
                    item.mask = LVIF_TEXT;
                    item.iSubItem = 1;
                    item.pszText = (LPWSTR) readText.c_str();
                    ListView_SetItem(notificationsList, &item);

                    itemNumber++;
                }
            );
        }

        /*
         * When a notification has been selected, mark it as the selected one
         */
        void NotificationsDialog::SelectNotification(HWND hwnd, NMLISTVIEW * details)
        {
            if ((details->uNewState ^ details->uOldState) & LVIS_SELECTED) {
                // Store the selected notification
                this->selectedNotification = details->iItem;

                LVITEM itemToRetrieve;
                itemToRetrieve.iItem = details->iItem;
                itemToRetrieve.iSubItem = 0;
                itemToRetrieve.mask = LVIF_PARAM;
                ListView_GetItem(GetDlgItem(hwnd, IDC_NOTIFICATIONS_LIST), (LPLVITEM) &itemToRetrieve);

                Notification * notification = reinterpret_cast<Notification *>(itemToRetrieve.lParam);

                if (notification == nullptr)
                {
                    LogWarning("Tried to select invalid notification");
                    return;
                }

                // Set the title
                std::wstring title = HelperFunctions::ConvertToWideString(notification->Title());
                SetDlgItemText(hwnd, IDC_NOTIFICATION_TITLE, title.c_str());

                // Set the body
                std::wstring body = HelperFunctions::ConvertToWideString(notification->Body());
                SetDlgItemText(hwnd, IDC_NOTIFICATION_BODY, body.c_str());

                // Handle the link
                if (notification->Link().empty())
                {
                    std::wstring link = L"";
                    SetDlgItemText(hwnd, IDC_NOTIFICATION_LINK, link.c_str());
                    ShowWindow(GetDlgItem(hwnd, IDC_NOTIFICATION_LINK), SW_HIDE);
                } else {
                    std::wstring link = L"<A HREF=\"";
                    link+= HelperFunctions::ConvertToWideString(notification->Link());
                    link+= L"\">Read More</A>";
                    SetDlgItemText(hwnd, IDC_NOTIFICATION_LINK, link.c_str());
                    ShowWindow(GetDlgItem(hwnd, IDC_NOTIFICATION_LINK), SW_SHOW);
                }
            }
        }

        /*
         * Mark the currently selected notification as read
         */
        void NotificationsDialog::MarkNotificationAsRead(HWND hwnd) const
        {
            if (this->selectedNotification == -1) {
                LogWarning("Tried to mark notification as read but none selected");
                return;
            }

            HWND notificationsList = GetDlgItem(hwnd, IDC_NOTIFICATIONS_LIST);

            // Get the row
            LVITEM itemToRetrieve;
            itemToRetrieve.iItem = this->selectedNotification;
            itemToRetrieve.iSubItem = 0;
            itemToRetrieve.mask = LVIF_PARAM;
            ListView_GetItem(notificationsList, (LPLVITEM) &itemToRetrieve);

            // Mark the notification as read
            Notification * notification = reinterpret_cast<Notification *>(itemToRetrieve.lParam);
            notification->Read();

            // Try to tell the API that has been read, but handle if it doesn't
            try
            {
                this->api.ReadNotification(notification->Id());
            } catch (Api::ApiException apiException)
            {
                LogError("ApiException when marking notification as read: " + std::string(apiException.what()));
            }

            // Update the notifications display to show that it's been read
            std::wstring readText = notification->IsRead() ? this->readString : this->unreadString;
            itemToRetrieve.mask = LVIF_TEXT;
            itemToRetrieve.iSubItem = 1;
            itemToRetrieve.pszText = (LPWSTR) readText.c_str();
            ListView_SetItem(notificationsList, &itemToRetrieve);

            // Force a repaint to make sure the notifications are painted the right colour
            RedrawWindow(notificationsList, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
        }

        HRESULT NotificationsDialog::HighlightRelevantNotification(HWND hwnd, LPNMLVCUSTOMDRAW customDraw)
        {
            const Notification* notification = reinterpret_cast<Notification*>(customDraw->nmcd.lItemlParam);

            if (
                activeCallsigns.UserHasCallsign() &&
                notification->IsRelevant(activeCallsigns.GetUserCallsign().GetNormalisedPosition()) &&
                !notification->IsRead()
            ) {
                customDraw->clrText = RGB(255, 0, 0);
            }

            return CDRF_NEWFONT;
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
