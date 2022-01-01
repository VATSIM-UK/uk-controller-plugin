#pragma once

namespace UKControllerPlugin {
    namespace Releases {

        class DepartureReleaseRequest;
        class DepartureReleaseEventHandler;

        /*
        *  Dialog for approving departure releases
         */
        class RejectDepartureReleaseDialog
        {
            public:
            explicit RejectDepartureReleaseDialog(std::shared_ptr<DepartureReleaseEventHandler> eventHandler);
            static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:

            void InitDialog(HWND hwnd, LPARAM lParam);
            void RejectRelease(HWND hwnd);
            LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            // The current release being handled
            std::shared_ptr<DepartureReleaseRequest> selectedRelease;

            // Handles release events
            std::shared_ptr<DepartureReleaseEventHandler> eventHandler;
            
            inline static const int REMARKS_BUFFER_SIZE = 255;
        };
    } // namespace Releases
}  // namespace UKControllerPlugin
