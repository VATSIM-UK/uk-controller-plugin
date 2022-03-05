#include "ParseTimeStrings.h"

namespace UKControllerPlugin::Time {

    const std::chrono::system_clock::time_point invalidTime = (std::chrono::system_clock::time_point::max)();
    const std::string FORMAT = "%F %T";
    const std::string ISO_8601_FORMAT = "%FT%T%Z";

    [[nodiscard]] auto ParseTimeString(const std::string& time) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(time);
        inputStream >> date::parse(FORMAT, timePoint);
        return static_cast<bool>(inputStream) ? timePoint : invalidTime;
    }

    auto ToDateTimeString(const std::chrono::system_clock::time_point& timePoint) -> std::string
    {
        return date::format(FORMAT, date::floor<std::chrono::seconds>(timePoint));
    }

    auto ParseIsoZuluString(const std::string& time) -> std::chrono::system_clock::time_point
    {
        date::sys_time<std::chrono::milliseconds> timePoint;
        std::istringstream inputStream(time);
        inputStream >> date::parse(ISO_8601_FORMAT, timePoint);
        return static_cast<bool>(inputStream) ? timePoint : invalidTime;
    }
} // namespace UKControllerPlugin::Time
