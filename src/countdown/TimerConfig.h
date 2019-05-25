#pragma once

namespace UKControllerPlugin {
    namespace Countdown {

        /*
            A struct representing the configuration of a particular countdown
            timer.
        */
        typedef struct TimerConfig {
            const unsigned int timerId;
            const bool timerEnabled;
            const unsigned int timerDuration;

            bool operator==(const TimerConfig & compare) const {
                return this->timerId == compare.timerId;
            }
        } TimeConfig;
    }  // namespace Countdown
}  // namespace UKControllerPlugin
