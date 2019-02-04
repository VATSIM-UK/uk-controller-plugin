#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        extern const unsigned int minimumVerticalSpeed;
        extern const unsigned int minimumLevelOffset;

        TCHAR * ConvertToTchar(std::string string);
        TCHAR * ConvertToTchar(int value);
        std::wstring GetLevelDisplayString(int altitude);
        unsigned int GetOccupiedLevel(int altitude, int verticalSpeed);
    }  // namespace Hold
}  // namespace UKControllerPlugin
