#pragma once
#include "countdown/TimerConfiguration.h"

namespace UKControllerPlugin {
    namespace Countdown {

        std::string GetTimerEnabledKey(const TimerConfiguration & configuration);
        std::string GetTimerEnabledKey(unsigned int timerId);
        std::string GetTimerEnabledDescription(const TimerConfiguration & configuration);
        std::string GetTimerEnabledDescription(unsigned int timerId);
        std::string GetTimerDurationKey(const TimerConfiguration & configuration);
        std::string GetTimerDurationKey(unsigned int timerId);
        std::string GetTimerDurationDescription(const TimerConfiguration & configuration);
        std::string GetTimerDurationDescription(unsigned int timerId);

    }  // namespace Countdown
}  // namespace UKControllerPlugin
