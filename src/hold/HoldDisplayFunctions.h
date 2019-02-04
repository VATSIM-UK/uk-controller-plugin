#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        TCHAR * ConvertToTchar(std::string string);
        TCHAR * ConvertToTchar(int value);
        std::wstring GetLevelDisplayString(unsigned int altitude);
    }  // namespace Hold
}  // namespace UKControllerPlugin
