#pragma once

namespace UKControllerPlugin {
    namespace HistoryTrail {
        struct HistoryTrailData;
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace HistoryTrail {
        /*
            This class handles the dialog box for changing history trail settings.
            It takes a pointer to a structure that in turn takes pointers to data - allowing
            it to change variables as required when it closes.
        */
        class HistoryTrailDialog
        {
        public:
            static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            // Maximum renderable trail length
            const int maxTrailLength = 99;

            // Minimum trail length
            const int minTrailLength = 1;

            // Default trail length
            const int defaultTrailLength = 15;

            // Highest dot size we can have
            const int minDotSize = 1;

            // Lowest dot size we can have
            const int maxDotSize = 10;

            // The minimum altitude on the altitude spinners
            const int minAltitudeValue = 0;

            // The max altitude
            const int maxAltitudeValue = 99999;

            // The default dotSize;
            const int defaultDotSize = 4;

            // Diamond trail type
            const int trailTypeDiamond = 0;

            // Square trail type
            const int trailTypeSquare = 1;

            // Circle trail type
            const int trailTypeCircle = 2;

            // Line trail type
            const int trailTypeLine = 3;

        private:
            LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            void InitDialog(HWND hwnd, LPARAM lParam);
            void OnOk(HWND hwnd);
            void ChoseTrailColour(HWND hwnd);
            void DrawColourButton(LPDRAWITEMSTRUCT drawItem);
            bool ValueInRange(int value, int min, int max);
            void TidyUpDialog(void);

            // Data to set up the dialog and return on close
            HistoryTrailData * data;

            // Stores colour data
            CHOOSECOLOR colourStruct{
                sizeof(CHOOSECOLOR),
                NULL,
                NULL,
                RGB(255, 255, 255),
                this->customTrailColours,
                CC_RGBINIT | CC_FULLOPEN,
                NULL,
                NULL,
                NULL
            };

            // Stores any custom trail colours
            COLORREF customTrailColours[16];

            // Brush for drawing the colour selector
            HBRUSH colourSelectorBrush;
        };
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
