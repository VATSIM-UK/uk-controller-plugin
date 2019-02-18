#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        extern const unsigned int minimumVerticalSpeed;
        extern const int minimumLevelOffset;

        TCHAR * ConvertToTchar(std::string string);
        TCHAR * ConvertToTchar(int value);
        std::wstring GetLevelDisplayString(int altitude);
        unsigned int GetOccupiedLevel(int altitude, int verticalSpeed);
        unsigned int GetDisplayRow(int holdMax, int occupiedLevel);
        std::wstring GetTimeInHoldDisplayString(const std::chrono::system_clock::time_point & entryTime);
    }  // namespace Hold
}  // namespace UKControllerPlugin
