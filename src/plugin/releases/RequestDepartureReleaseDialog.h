#pragma once

namespace UKControllerPlugin {

    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope

    namespace Releases {

        class DepartureReleaseEventHandler;

        /*
         *  Dialog for requesting departure releases
         */
        class RequestDepartureReleaseDialog
        {
            public:
            RequestDepartureReleaseDialog(
                std::shared_ptr<DepartureReleaseEventHandler> eventHandler,
                const Controller::ActiveCallsignCollection& activeCallsigns,
                Euroscope::EuroscopePluginLoopbackInterface& plugin);
            static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            private:
            void InitDialog(HWND hwnd, LPARAM lParam);
            void RequestRelease(HWND hwnd);
            LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            std::string selectedCallsign = "";

            // All the active callsigns
            const Controller::ActiveCallsignCollection& activeCallsigns;

            // The plugin for controller interaction
            Euroscope::EuroscopePluginLoopbackInterface& plugin;

            // Deals with events related to the releases
            const std::shared_ptr<DepartureReleaseEventHandler> eventHandler;
        };
    } // namespace Releases
} // namespace UKControllerPlugin
