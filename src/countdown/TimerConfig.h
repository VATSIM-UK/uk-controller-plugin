#pragma once

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A struct representing the configuration of a particular countdown
            timer.
        */
        typedef struct TimerConfig 
        {
            // The id of the timer
            const unsigned int timerId;

            // Whether or not the timer is enabled / visible
            const bool timerEnabled;

            // How long the timer should last for
            const unsigned int timerDuration;

            bool operator==(const TimerConfig & compare) const
            {
                return this->timerId == compare.timerId;
            }

            bool operator<(const TimerConfig & compare) const
            {
                return this->timerId < compare.timerId;
            }
        } TimeConfig;
    }  // namespace Countdown
}  // namespace UKControllerPlugin
