#pragma once

namespace UKControllerPlugin {
    namespace Time {
        std::chrono::system_clock::time_point TimeNow();
        void SetTestNow(std::chrono::system_clock::time_point now);
    } // namespace Time
} // namespace UKControllerPlugin
