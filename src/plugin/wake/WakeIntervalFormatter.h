#pragma once

namespace UKControllerPlugin::Wake {
    class WakeIntervalInterface;

    [[nodiscard]] auto FormatInterval(const WakeIntervalInterface& interval) -> std::wstring;
} // namespace UKControllerPlugin::Wake
