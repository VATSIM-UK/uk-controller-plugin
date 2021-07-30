#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Countdown {
        /*
            This class forms a countdown timer that will count down
            a specific amount of time.
        */
        class CountdownTimer
        {
        public:
            explicit CountdownTimer(UKControllerPlugin::Windows::WinApiInterface & winApi);
            virtual ~CountdownTimer(void);
            std::wstringstream GetCurrentTimeString(int secondsRemaining);
            int GetSecondsRemaining(void);
            virtual void ResetTimer(bool playSound);
            virtual void StartTimer(time_t duration);

        private:

            // The time (seconds since UNIX epoch) at which the timer was started
            time_t endTime = 0;

            // Windows API
            UKControllerPlugin::Windows::WinApiInterface & winApi;
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin
