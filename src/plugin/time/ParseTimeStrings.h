#pragma once

namespace UKControllerPlugin::Time {
    [[nodiscard]] auto ParseTimeString(const std::string& time) -> std::chrono::system_clock::time_point;
    extern const std::chrono::system_clock::time_point invalidTime;
    [[nodiscard]] auto ToDateTimeString(const std::chrono::system_clock::time_point& timePoint) -> std::string;
    [[nodiscard]] auto ParseIsoZuluString(const std::string& time) -> std::chrono::system_clock::time_point;
    extern const std::chrono::system_clock::time_point invalidTime;
} // namespace UKControllerPlugin::Time
