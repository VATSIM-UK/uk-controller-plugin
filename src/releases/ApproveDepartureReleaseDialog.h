#pragma once

namespace UKControllerPlugin {
    namespace Releases {

        class DepartureReleaseRequest;
        class DepartureReleaseEventHandler;

        /*
        *  Dialog for approving departure releases
        */
        class ApproveDepartureReleaseDialog
        {
            public:
                explicit ApproveDepartureReleaseDialog(std::shared_ptr<DepartureReleaseEventHandler> eventHandler);
                static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

                void InitDialog(HWND hwnd, LPARAM lParam);
                void ApproveRelease(HWND hwnd);
                LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

                // The current release being handled
                std::shared_ptr<DepartureReleaseRequest> selectedRelease;

                // Handles release events
                std::shared_ptr<DepartureReleaseEventHandler> eventHandler;

                // The valid times for releases
                const std::map<int, std::wstring> releaseValidForTimes = {
                    {-1, L"Indefinite"},
                    {60, L"1 Minute"},
                    {90, L"90 Seconds"},
                    {120, L"2 Minutes"},
                    {180, L"3 Minutes"},
                    {240, L"4 Minutes"},
                    {300, L"5 Minutes"},
                    {600, L"10 Minutes"},
                };
        };
    } // namespace Releases
}  // namespace UKControllerPlugin
