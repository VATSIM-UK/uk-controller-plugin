#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplay::HoldDisplay(HWND parent)
        {
            const long groupX = GetHoldGroupX(0);
            const long groupY = GetHoldGroupY(0);

            this->identifierHandle = CreateStaticLine(
                parent,
                GetHoldIdentifierDimensions(groupX, groupY),
                L"Identifier: TIMBA"
            );

            this->inboundHeadingHandle = CreateStaticLine(
                parent,
                GetHoldInboundDimensions(groupX, groupY),
                L"Inbound: 309"
            );

            this->minimumAltitudeHandle = CreateStaticLine(
                parent,
                GetHoldMinimumDimensions(groupX, groupY),
                L"Minimum: 7000"
            );

            this->maximumAltitudeHandle = CreateStaticLine(
                parent,
                GetHoldMaximumDimensions(groupX, groupY),
                L"Maximum: 15000"
            );

            this->turnDirectionHandle = CreateStaticLine(
                parent,
                GetHoldTurnDimensions(groupX, groupY),
                L"Turn: Right"
            );

            this->tableHandle = CreateHoldList(
                parent,
                GetHoldTableDimensions(groupX, groupY)
            );
        }

        HoldDisplay::~HoldDisplay()
        {
            if (this->identifierHandle) {
                DestroyWindow(this->identifierHandle);
            }

            if (this->inboundHeadingHandle) {
                DestroyWindow(this->inboundHeadingHandle);
            }

            if (this->minimumAltitudeHandle) {
                DestroyWindow(this->minimumAltitudeHandle);
            }

            if (this->maximumAltitudeHandle) {
                DestroyWindow(this->maximumAltitudeHandle);
            }

            if (this->turnDirectionHandle) {
                DestroyWindow(this->turnDirectionHandle);
            }

            if (this->tableHandle) {
                DestroyWindow(this->tableHandle);
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
