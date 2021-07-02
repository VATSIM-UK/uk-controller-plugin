#include "utils/pch.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin {
    namespace Time {

        std::chrono::system_clock::time_point defaultTestNow = (std::chrono::system_clock::time_point::min)();
        std::chrono::system_clock::time_point testNowTime = (std::chrono::system_clock::time_point::min)();

        std::chrono::system_clock::time_point TimeNow()
        {
            return testNowTime != defaultTestNow
                       ? testNowTime
                       : std::chrono::system_clock::now();
        }

        void SetTestNow(std::chrono::system_clock::time_point now)
        {
            testNowTime = now;
        }
    } // namespace Time
} // namespace UKControllerPlugin
