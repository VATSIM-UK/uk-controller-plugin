#pragma once

namespace UKControllerPlugin::Wake {
    struct DepartureWakeInterval;

    [[nodiscard]] auto FormatInterval(const DepartureWakeInterval& interval) -> std::wstring;
} // namespace UKControllerPlugin::Wake
