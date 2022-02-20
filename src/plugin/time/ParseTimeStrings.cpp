#include "ParseTimeStrings.h"

namespace UKControllerPlugin::Time {

    const std::chrono::system_clock::time_point invalidTime = (std::chrono::system_clock::time_point::max)();
    const std::string FORMAT = "%F %T";

    [[nodiscard]] auto ParseTimeString(const std::string& time) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(time);
        inputStream >> std::chrono::parse(FORMAT, timePoint);
        return static_cast<bool>(inputStream) ? timePoint : invalidTime;
    }

    auto ToDateTimeString(const std::chrono::system_clock::time_point& timePoint) -> std::string
    {
        return fmt::format(FORMAT, std::chrono::floor<std::chrono::seconds>(timePoint));
    }
} // namespace UKControllerPlugin::Time
