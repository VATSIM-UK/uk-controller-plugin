#pragma once

namespace UKControllerPlugin {
    namespace Releases {
        /*
         * The colour to show when counting down to release time.
         */
        COLORREF PendingReleaseTimeColour();

        /*
         * Colour to show when counting down to expiry time.
         */
        COLORREF TimeUntilExpiryColour(const std::chrono::system_clock::time_point& expiry);
    } // namespace Releases
} // namespace UKControllerPlugin
