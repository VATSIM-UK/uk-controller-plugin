#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        extern const int minimumVerticalSpeed;
        extern const int minimumLevelOffset;

        std::wstring ConvertToTchar(std::string string);
        std::wstring ConvertToTchar(int value);
        std::string ConvertFromTchar(TCHAR* string);
        std::wstring GetLevelDisplayString(int altitude);
        unsigned int GetOccupiedLevel(int altitude, int verticalSpeed);
        unsigned int GetDisplayRow(int holdMax, int occupiedLevel);
        std::wstring GetTimeInHoldDisplayString(const std::chrono::system_clock::time_point& entryTime);
        int GetVerticalSpeedDirection(int verticalSpeed);
        [[nodiscard]] auto FormatOutboundLegValue(double value) -> std::wstring;
    } // namespace Hold
} // namespace UKControllerPlugin
