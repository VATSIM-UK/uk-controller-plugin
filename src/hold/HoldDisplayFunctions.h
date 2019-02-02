#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        HWND CreateProfileSelector(HWND parent);
        TCHAR * ConvertToTchar(std::string string);
        TCHAR * ConvertToTchar(int value);
        UKControllerPlugin::Hold::HoldElementDimensions GetProfileSelectorDimensions(RECT parent);
        void MoveProfileSelector(HWND handle, HWND parent);
    }  // namespace Hold
}  // namespace UKControllerPlugin
