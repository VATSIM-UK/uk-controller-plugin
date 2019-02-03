#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Manages the display of a single hold.
        */
        class HoldDisplay
        {
            public:
                HoldDisplay(HWND parent);
                ~HoldDisplay();

            private:

                // Handle to the control that displays the hold identifier
                HWND identifierHandle;

                // Handle to the control that displays the hold inbound heading
                HWND inboundHeadingHandle;

                // Handle to the control that displays the hold minimum altitude
                HWND minimumAltitudeHandle;

                // Handle to the control that displays the hold maximum altitude
                HWND maximumAltitudeHandle;

                // Handle to the control that displays the hold turn direction
                HWND turnDirectionHandle;

                // Hold table handle
                HWND tableHandle;

                // The handle to the static display font
                HFONT staticDisplayFont;

                // The properties of the static display font
                LOGFONT staticFontProps = { 0 };
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
