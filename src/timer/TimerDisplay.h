#pragma once

namespace UKControllerPlugin {
    namespace Timer {
        std::wstring GetTimerDisplayWide(std::chrono::system_clock::time_point targetTime);
        std::string GetTimerDisplay(std::chrono::system_clock::time_point targetTime);
    } // namespace Timer
} // namespace UKControllerPlugin
