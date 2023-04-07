#include "countdown/GlobalCountdownSettingFunctions.h"

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledKey(const TimerConfiguration& configuration)
        {
            return GetTimerEnabledKey(configuration.timerId);
        }

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledKey(unsigned int timerId)
        {
            return "timer" + std::to_string(timerId) + "Enabled";
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledDescription(const TimerConfiguration& configuration)
        {
            return GetTimerEnabledDescription(configuration.timerId);
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerEnabledDescription(unsigned int timerId)
        {
            return "Countdown Timer #" + std::to_string(timerId) + " Enabled";
        }

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationKey(const TimerConfiguration& configuration)
        {
            return GetTimerDurationKey(configuration.timerId);
        }

        /*
            Get the settings key for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationKey(unsigned int timerId)
        {
            return "timer" + std::to_string(timerId) + "Duration";
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationDescription(const TimerConfiguration& configuration)
        {
            return GetTimerDurationDescription(configuration.timerId);
        }

        /*
            Get the settings description for whether or not the timer is
            enabled.
        */
        std::string GetTimerDurationDescription(unsigned int timerId)
        {
            return "Countdown Timer #" + std::to_string(timerId) + " Duration";
        }

    } // namespace Countdown
} // namespace UKControllerPlugin
