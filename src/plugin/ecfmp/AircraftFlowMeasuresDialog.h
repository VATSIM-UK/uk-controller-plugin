#pragma once

namespace UKControllerPlugin::ECFMP {

    struct AircraftFlowMeasuresDialogData
    {
        // The aircraft callsign
        std::wstring callsign;

        // The description of the flow measures
        std::wstring flowMeasures;
    };

    class AircraftFlowMeasuresDialog
    {
        public:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        private:
        LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void InitDialog(HWND hwnd, LPARAM lParam);
    };
} // namespace UKControllerPlugin::ECFMP
