#include "pch/stdafx.h"
#include "releases/ApproveDepartureReleaseDialog.h"
#include "DepartureReleaseEventHandler.h"
#include "dialog/DialogCallArgument.h"
#include "helper/HelperFunctions.h"
#include "releases/DepartureReleaseRequest.h"
#include "time/SystemClock.h"
#include "time/ParseTimeStrings.h"

namespace UKControllerPlugin {
    namespace Releases {

        ApproveDepartureReleaseDialog::ApproveDepartureReleaseDialog(
            std::shared_ptr<DepartureReleaseEventHandler> eventHandler
        ): eventHandler(std::move(eventHandler)
        )
        { }

        /*
    Private dialog procedure for the dialog, should be used
    against a bound instance.
*/
        LRESULT ApproveDepartureReleaseDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                            this->ApproveRelease(hwnd);
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDCANCEL: {
                            EndDialog(hwnd, wParam);
                            return TRUE;
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
        LRESULT ApproveDepartureReleaseDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("Approve departure releases dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<Dialog::DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Approve departure releases dialog closed");
            }

            ApproveDepartureReleaseDialog* dialog = reinterpret_cast<ApproveDepartureReleaseDialog*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
         * Set up the notifications list
         */
        void ApproveDepartureReleaseDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            // Get the aircraft callsign from dialog argument
            this->selectedRelease = *reinterpret_cast<std::shared_ptr<DepartureReleaseRequest>*>(
                reinterpret_cast<Dialog::DialogCallArgument*>(lParam)->contextArgument
            );

            // Set the aircraft callsign box
            std::wstring wideCallsign = HelperFunctions::ConvertToWideString(this->selectedRelease->Callsign());

            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_CALLSIGN,
                WM_SETTEXT,
                NULL,
                reinterpret_cast<LPARAM>(wideCallsign.c_str())
            );

            // Add the valid for options
            for (auto validForTime : this->releaseValidForTimes) {
                std::wstring validForDisplayString = validForTime.second;
                int validForSeconds = validForTime.first;
                // Set the data
                int itemIndex = SendDlgItemMessage(
                    hwnd,
                    IDC_DEPARTURE_RELEASE_APPROVE_VALIDFOR,
                    CB_ADDSTRING,
                    NULL,
                    reinterpret_cast<LPARAM>(validForDisplayString.c_str())
                );

                SendDlgItemMessage(
                    hwnd,
                    IDC_DEPARTURE_RELEASE_APPROVE_VALIDFOR,
                    CB_SETITEMDATA,
                    itemIndex,
                    validForSeconds
                );
            }
            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_VALIDFOR,
                CB_SETCURSEL,
                0,
                NULL
            );

            // Set released at format
            std::wstring format = L"HH':'mm";
            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_RELEASED_AT,
                DTM_SETFORMAT,
                NULL,
                reinterpret_cast<LPARAM>(format.c_str())
            );

            // Set released at min/max
            SYSTEMTIME minMaxTimes[2];

            // Min time is now
            auto currentTime = Time::TimeNow();
            auto dp = date::floor<date::days>(currentTime);
            auto ymd = date::year_month_day(dp);
            auto time = date::make_time(std::chrono::duration_cast<std::chrono::minutes>(currentTime - dp));
            SYSTEMTIME minTime;
            minTime.wYear = static_cast<int>(ymd.year());
            minTime.wMonth = static_cast<unsigned>(ymd.month());
            minTime.wDay = static_cast<unsigned>(ymd.day());
            minTime.wDayOfWeek = static_cast<unsigned>(ymd.day()) % 7;
            minTime.wHour = time.hours().count();
            minTime.wMinute = time.minutes().count();
            minTime.wSecond = 0;
            minTime.wMilliseconds = 0;
            minMaxTimes[0] = minTime;

            // Set the control default
            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_RELEASED_AT,
                DTM_SETSYSTEMTIME,
                GDT_VALID,
                reinterpret_cast<LPARAM>(&minTime)
            );

            // Max time is 10 minutes from now
            auto timeInTenMinutes = Time::TimeNow() + std::chrono::minutes(10);
            auto dpTenMinutes = date::floor<date::days>(timeInTenMinutes);
            auto timeTenMinutes = date::make_time(
                std::chrono::duration_cast<std::chrono::minutes>(timeInTenMinutes - dpTenMinutes)
            );
            SYSTEMTIME maxTime;
            maxTime.wYear = static_cast<int>(ymd.year());
            maxTime.wMonth = static_cast<unsigned>(ymd.month());
            maxTime.wDay = static_cast<unsigned>(ymd.day());
            maxTime.wDayOfWeek = static_cast<unsigned>(ymd.day()) % 7;
            maxTime.wHour = timeTenMinutes.hours().count();
            maxTime.wMinute = timeTenMinutes.minutes().count();
            maxTime.wSecond = 0;
            maxTime.wMilliseconds = 0;
            minMaxTimes[1] = maxTime;

            // Set them
            SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_RELEASED_AT,
                DTM_SETRANGE,
                GDTR_MIN | GDTR_MAX,
                reinterpret_cast<LPARAM>(minMaxTimes)
            );
        }

        void ApproveDepartureReleaseDialog::ApproveRelease(HWND hwnd)
        {
            // Get the duration
            int selectedDuration = SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_VALIDFOR,
                CB_GETCURSEL,
                NULL,
                NULL
            );

            if (selectedDuration == CB_ERR) {
                return;
            }

            // Get the duration
            int durationSeconds = SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_VALIDFOR,
                CB_GETITEMDATA,
                selectedDuration,
                NULL
            );

            SYSTEMTIME releasedAtTime;
            LRESULT releasedAtTimeValid = SendDlgItemMessage(
                hwnd,
                IDC_DEPARTURE_RELEASE_APPROVE_RELEASED_AT,
                DTM_GETSYSTEMTIME,
                NULL,
                reinterpret_cast<LPARAM>(&releasedAtTime)
            );

            if (releasedAtTimeValid != GDT_VALID) {
                return;
            }

            // Approve the release request
            std::tm tm;
            tm.tm_sec = releasedAtTime.wSecond;
            tm.tm_min = releasedAtTime.wMinute;
            tm.tm_hour = releasedAtTime.wHour;
            tm.tm_mday = releasedAtTime.wDay;
            tm.tm_mon = releasedAtTime.wMonth - 1;
            tm.tm_year = releasedAtTime.wYear - 1900;
            tm.tm_isdst = -1;
            eventHandler->ApproveRelease(
                this->selectedRelease->Id(),
                std::chrono::system_clock::from_time_t(std::mktime(&tm)),
                durationSeconds
            );
        }
    } // namespace Releases
}  // namespace UKControllerPlugin
