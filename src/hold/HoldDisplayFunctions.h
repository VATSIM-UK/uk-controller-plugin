#pragma once
#include "hold/HoldElementDimensions.h"

namespace UKControllerPlugin {
    namespace Hold {

        extern const int columnWidth;
        extern const int staticHeight;
        extern const int holdGap;

        HWND CreateProfileSelector(HWND parent);
        TCHAR * ConvertToTchar(std::string string);
        TCHAR * ConvertToTchar(int value);
        HWND CreateHoldList(HWND parent, UKControllerPlugin::Hold::HoldElementDimensions dimensions);
        HWND CreateStaticLine(HWND parent, UKControllerPlugin::Hold::HoldElementDimensions dimensions, LPCWSTR text);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldIdentifierDimensions(long groupX, long groupY);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldInboundDimensions(long groupX, long groupY);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldMinimumDimensions(long groupX, long groupY);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldMaximumDimensions(long groupX, long groupY);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldTurnDimensions(long groupX, long groupY);
        UKControllerPlugin::Hold::HoldElementDimensions GetHoldTableDimensions(long groupX, long groupY);
        long GetHoldGroupX(int holdNumber);
        long GetHoldGroupY(int holdNumber);
        UKControllerPlugin::Hold::HoldElementDimensions GetProfileSelectorDimensions(RECT parent);
        void MoveProfileSelector(HWND handle, HWND parent);
    }  // namespace Hold
}  // namespace UKControllerPlugin
