#pragma once
#include "pch/stdafx.h"
#include "countdown/TimerConfiguration.h"

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledKey(TimerConfiguration configuration)
        {
            return "timer" + std::to_string(configuration.timerId) + "Enabled";
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledDescription(TimerConfiguration configuration)
        {
            return "Countdown Timer #" + std::to_string(configuration.timerId) + " Enabled";
        }

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationKey(TimerConfiguration configuration)
        {
            return "timer" + std::to_string(configuration.timerId) + "Duration";
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationDescription(TimerConfiguration configuration)
        {
            return "Countdown Timer #" + std::to_string(configuration.timerId) + " Duration";
        }

    }  // namespace Countdown
}  // namespace UKControllerPlugin
