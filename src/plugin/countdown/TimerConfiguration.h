#pragma once

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A struct representing the configuration of a particular countdown
            timer.
        */
        typedef struct TimerConfiguration
        {
            // The id of the timer
            const unsigned int timerId;

            // Whether or not the timer is enabled / visible
            const bool timerEnabled;

            // How long the timer should last for
            const unsigned int timerDuration;

            bool operator==(const TimerConfiguration& compare) const
            {
                return this->timerId == compare.timerId;
            }

            bool operator<(const TimerConfiguration& compare) const
            {
                return this->timerId < compare.timerId;
            }
        } TimerConfiguration;
    } // namespace Countdown
} // namespace UKControllerPlugin
