#pragma once

namespace UKControllerPlugin {
    namespace Time {
        std::chrono::system_clock::time_point ParseTimeString(std::string time);
        extern const std::chrono::system_clock::time_point invalidTime;
    }  // namespace Time
}  // namespace UKControllerPlugin
