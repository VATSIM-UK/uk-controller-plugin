#include "pch/pch.h"
#include "timer/TimerDisplay.h"
#include "helper/HelperFunctions.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin {
    namespace Timer {
        std::wstring GetTimerDisplayWide(std::chrono::system_clock::time_point targetTime)
        {
            return HelperFunctions::ConvertToWideString(GetTimerDisplay(targetTime));
        }

        std::string GetTimerDisplay(std::chrono::system_clock::time_point targetTime)
        {
            std::chrono::system_clock::time_point now = Time::TimeNow();
            if (targetTime <= now) {
                return "0:00";
            }

            std::chrono::duration minutesRemaining =
                std::chrono::duration_cast<std::chrono::minutes>(targetTime - now);

            std::chrono::duration secondsRemaining =
                std::chrono::duration_cast<std::chrono::seconds>(targetTime - now) % 60;

            std::string secondsString = secondsRemaining.count() >= 10
                                            ? std::to_string(secondsRemaining.count())
                                            : "0" + std::to_string(secondsRemaining.count());

            return std::to_string(minutesRemaining.count()) + ":" + secondsString;
        }
    } // namespace Timer
} // namespace UKControllerPlugin
