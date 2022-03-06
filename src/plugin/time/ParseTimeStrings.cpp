#include "ParseTimeStrings.h"

namespace UKControllerPlugin::Time {

    const std::chrono::system_clock::time_point invalidTime = (std::chrono::system_clock::time_point::max)();

    [[nodiscard]] auto ParseTimeString(const std::string& time) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(time);
        inputStream >> std::chrono::parse(std::string{"%F %T"}, timePoint);
        return static_cast<bool>(inputStream) ? timePoint : invalidTime;
    }

    auto ToDateTimeString(const std::chrono::system_clock::time_point& timePoint) -> std::string
    {
        return fmt::format("{:%F %T}", fmt::gmtime(std::chrono::floor<std::chrono::seconds>(timePoint)));
    }

    auto ParseIsoZuluString(const std::string& time) -> std::chrono::system_clock::time_point
    {
        std::chrono::system_clock::time_point timePoint;
        std::istringstream inputStream(time);
        inputStream >> std::chrono::parse(std::string{"%FT%T%Z"}, timePoint);
        return static_cast<bool>(inputStream) ? timePoint : invalidTime;
    }
} // namespace UKControllerPlugin::Time
