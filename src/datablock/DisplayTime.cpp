#include "pch/stdafx.h"
#include "datablock/DisplayTime.h"

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Converts a timestamp to its Zulu string representation for display.
        */
        std::string DisplayTime::FromTimestamp(time_t time) const
        {
            return date::format(this->timeFormat, std::chrono::system_clock::from_time_t(time));
        }

        /*
            Converts the current system time in UTC to its zulu string representation
            for display.
        */
        std::string DisplayTime::FromSystemTime(void) const
        {
            return date::format(this->timeFormat, std::chrono::system_clock::now());
        }

        /*
            Converts the current system time in UTC to its zulu string representation
            for display.
        */
        std::string DisplayTime::FromTimePoint(std::chrono::system_clock::time_point tp) const
        {
            return date::format(this->timeFormat, tp);
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin
