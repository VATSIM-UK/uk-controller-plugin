#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplay::HoldDisplay(HWND parent)
        {
            // Setup the font
            this->staticFontProps.lfHeight = 16;
            this->staticFontProps.lfWidth = 8;
            this->staticFontProps.lfWeight = FW_MEDIUM;
            this->staticFontProps.lfCharSet = DEFAULT_CHARSET;
            this->staticFontProps.lfOutPrecision = OUT_DEFAULT_PRECIS;
            this->staticFontProps.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            this->staticFontProps.lfQuality = ANTIALIASED_QUALITY;
            this->staticFontProps.lfPitchAndFamily = DEFAULT_QUALITY | FF_MODERN;
            this->staticDisplayFont = CreateFontIndirect(&this->staticFontProps);

            // Get the group position
            const long groupX = GetHoldGroupX(0);
            const long groupY = GetHoldGroupY(0);

            // Create the static elements
            this->identifierHandle = CreateStaticLine(
                parent,
                GetHoldIdentifierDimensions(groupX, groupY),
                L"Identifier: TIMBA"
            );
            SendMessage(this->identifierHandle, WM_SETFONT, (WPARAM) this->staticDisplayFont, TRUE);

            this->inboundHeadingHandle = CreateStaticLine(
                parent,
                GetHoldInboundDimensions(groupX, groupY),
                L"Inbound: 309"
            );
            SendMessage(this->inboundHeadingHandle, WM_SETFONT, (WPARAM)this->staticDisplayFont, TRUE);

            this->minimumAltitudeHandle = CreateStaticLine(
                parent,
                GetHoldMinimumDimensions(groupX, groupY),
                L"Minimum: 7000"
            );
            SendMessage(this->minimumAltitudeHandle, WM_SETFONT, (WPARAM)this->staticDisplayFont, TRUE);

            this->maximumAltitudeHandle = CreateStaticLine(
                parent,
                GetHoldMaximumDimensions(groupX, groupY),
                L"Maximum: 15000"
            );
            SendMessage(this->maximumAltitudeHandle, WM_SETFONT, (WPARAM)this->staticDisplayFont, TRUE);

            this->turnDirectionHandle = CreateStaticLine(
                parent,
                GetHoldTurnDimensions(groupX, groupY),
                L"Turn: Right"
            );
            SendMessage(this->turnDirectionHandle, WM_SETFONT, (WPARAM)this->staticDisplayFont, TRUE);

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

            if (this->staticDisplayFont) {
                DeleteObject(this->staticDisplayFont);
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
