#include "pch/stdafx.h"
#include "time/ParseTimeStrings.h"

namespace UKControllerPlugin {
    namespace Time {

        const std::chrono::system_clock::time_point invalidTime = (std::chrono::system_clock::time_point::max)();

        std::chrono::system_clock::time_point ParseTimeString(std::string time)
        {
            std::chrono::system_clock::time_point timePoint;
            std::istringstream inputStream(time);
            inputStream >> date::parse("%Y-%m-%d %H:%M:%S", timePoint);
            return static_cast<bool>(inputStream) ? timePoint : invalidTime;
        }
    }  // namespace Time
}  // namespace UKControllerPlugin
