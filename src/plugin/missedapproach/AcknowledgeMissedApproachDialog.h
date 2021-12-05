#pragma once

namespace UKControllerPlugin::MissedApproach {
    class AcknowledgeMissedApproachTagFunction;

    /**
     * Configures missed approaches.
     */
    class AcknowledgeMissedApproachDialog
    {
        public:
        AcknowledgeMissedApproachDialog(std::shared_ptr<AcknowledgeMissedApproachTagFunction> tagFunction);
        static auto CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

        private:
        auto PrivateWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
        auto InitDialog(HWND hwnd, LPARAM lParam) -> LRESULT;
        auto SaveDialog(HWND hwnd) -> LRESULT;

        std::shared_ptr<AcknowledgeMissedApproachTagFunction> tagFunction;

        private:
        static inline const int REMARKS_BUFFER = 255;
    };
} // namespace UKControllerPlugin::MissedApproach
