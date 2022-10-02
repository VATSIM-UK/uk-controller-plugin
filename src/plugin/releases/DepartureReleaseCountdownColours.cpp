#include "releases/DepartureReleaseCountdownColours.h"
#include "releases/DepartureReleaseColours.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin {
    namespace Releases {
        COLORREF PendingReleaseTimeColour()
        {
            return statusIndicatorReleasedAwaitingTime;
        }

        COLORREF TimeUntilExpiryColour(const std::chrono::system_clock::time_point& expiry)
        {
            int64_t secondsRemaining =
                std::chrono::duration_cast<std::chrono::seconds>(expiry - Time::TimeNow()).count();

            if (secondsRemaining > 30) {
                return releaseTimerPlentyOfTime;
            }

            if (secondsRemaining > 10) {
                return releaseTimerClose;
            }

            return releaseTimerExpired;
        }
    } // namespace Releases
} // namespace UKControllerPlugin
