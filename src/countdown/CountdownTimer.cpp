#include "pch/stdafx.h"
#include "countdown/CountdownTimer.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
namespace UKControllerPlugin {
    namespace Countdown {
        CountdownTimer::CountdownTimer(UKControllerPlugin::Windows::WinApiInterface & winApi)
            : winApi(winApi)
        {

        }

        CountdownTimer::~CountdownTimer(void)
        {

        }

        /*
            Given a number of seconds remaining, return the time display in the
            form of a wide string stream. This is required as Euroscope only
            takes LPCWSTR.
        */
        std::wstringstream CountdownTimer::GetCurrentTimeString(int secondsRemaining)
        {
            int minutes = secondsRemaining / 60;
            int seconds = secondsRemaining % 60;

            // Use a WideStringStream to convert this data eventually into LPCWSTR
            std::wstringstream wss;

            if (seconds < 10) {
                wss << minutes << ":0" << seconds << '\0';
            } else {
                wss << minutes << ":" << seconds << '\0';
            }

            return wss;
        }

        /*
            Compare the current time to the timers end time.

            Return the difference in seconds between the two times, or zero
            if the timer has ended.
        */
        int CountdownTimer::GetSecondsRemaining(void)
        {
            if (this->endTime == NULL) {
                return 0;
            }

            int timeDiff = static_cast<int>(difftime(endTime, time(0)));
            if (timeDiff <= 0) {
                this->ResetTimer(true);
                return 0;
            }

            return timeDiff;
        }

        /*
            Resets the timer to zero and plays a sound to alert
            the user if requested.
        */
        void CountdownTimer::ResetTimer(bool playSound)
        {
            if (this->endTime != NULL) {
                if (playSound) {
                    this->winApi.PlayWave(MAKEINTRESOURCE(IDR_WAVE1));
                }

                this->endTime = NULL;
            }
        }

        /*
            Start the timer, set the start time and
            how long it should go for.
        */
        void CountdownTimer::StartTimer(time_t duration)
        {
            this->endTime = time(0) + duration;
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
