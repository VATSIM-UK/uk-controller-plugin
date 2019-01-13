#pragma once

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            A class for converting times to string representations, providing
            a wrapper for Howard Hinnant's date library.
        */
        class DisplayTime
        {
            public:
                std::string FromTimestamp(time_t time) const;
                std::string FromSystemTime(void) const;
                std::string FromTimePoint(std::chrono::system_clock::time_point tp) const;

                // The format for time strings
                const std::string timeFormat = "%H:%M";
        };
    }  // namespace Datablock
}  // namespace UKControllerPlugin
