#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/ManagedHold.h"

namespace UKControllerPlugin {
    namespace Hold {

        bool HoldDisplay::windowRegistered = false;

        HoldDisplay::HoldDisplay(
            HWND euroscopeWindow,
            HINSTANCE dllInstance,
            const ManagedHold & managedHold
        )
            : managedHold(managedHold),
            titleBarBrush(Gdiplus::Color(255, 153, 153)), backgroundBrush(CreateSolidBrush(RGB(0, 0, 0))),
            titleBarTextBrush(Gdiplus::Color(255, 255, 255)), fontFamily(L"EuroScope"),
            font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip),
            dataBrush(Gdiplus::Color(0, 176, 0)),
            clearedLevelBrush(Gdiplus::Color(255, 128, 64)),
            borderPen(Gdiplus::Color(255, 255, 255), 1.5f),
            exitButtonBrush(Gdiplus::Color(0, 0, 0)),
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255))
        {
         
            if (!this->windowRegistered) {
                RegisterWindowClass(dllInstance);
            }

            this->stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

            this->selfHandle = CreateWindow(
                windowClassName,
                L"UKCP Hold Manager",
                WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                this->windowWidth,
                this->windowHeight,
                euroscopeWindow,
                NULL,
                dllInstance,
                (LPVOID) this
            );

            if (!this->selfHandle) {
                LogError("Unable to create hold display");
                return;
            }

            SetWindowLong(this->selfHandle, GWL_STYLE, 0);
            ShowWindow(this->selfHandle, 5);
        }

        HoldDisplay::HoldDisplay(const HoldDisplay & copy)
            : managedHold(copy.managedHold),
            titleBarBrush(Gdiplus::Color(255, 153, 153)),
            backgroundBrush(CreateSolidBrush(RGB(0, 0, 0))),
            titleBarTextBrush(Gdiplus::Color(255, 255, 255)), fontFamily(L"EuroScope"),
            font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip),
            dataBrush(Gdiplus::Color(0, 176, 0)),
            clearedLevelBrush(Gdiplus::Color(255, 128, 64)),
            borderPen(Gdiplus::Color(255, 255, 255), 1.5f),
            exitButtonBrush(Gdiplus::Color(0, 0, 0)),
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255))
        {
            this->selfHandle = copy.selfHandle;
        }

        HoldDisplay::~HoldDisplay()
        {
            if (this->selfHandle) {
                DeleteObject(this->selfHandle);
                this->selfHandle = NULL;
            }

            if (this->backgroundBrush) {
                DeleteObject(this->backgroundBrush);
            }
        }

        void HoldDisplay::PaintWindow(HDC hdc)
        {
            Gdiplus::Graphics graphics(hdc);
            RECT windowRect;
            GetClientRect(this->selfHandle, &windowRect);

            // Title bar
            graphics.FillRectangle(&this->titleBarBrush, this->titleArea);
            graphics.DrawString(L"TIMBA", 5, &this->font, this->titleArea, &this->stringFormat, &this->titleBarTextBrush);
            graphics.DrawLine(
                &this->borderPen,
                this->titleArea.X,
                this->titleArea.Y + this->titleArea.Height,
                this->titleArea.X + this->titleArea.Width,
                this->titleArea.Y + this->titleArea.Height
            );

            // Exit Button
            graphics.FillRectangle(&this->exitButtonBrush, this->exitButtonArea);
            graphics.DrawString(L"X", 1, &this->font, this->exitButtonArea, &this->stringFormat, &this->titleBarTextBrush);

            // Hold display
            Gdiplus::RectF numbersDisplay = {
                0.0f,
                this->dataStartHeight,
                30.0f,
                15.00f
            };

            Gdiplus::RectF callsignDisplay = {
                35.0f,
                this->dataStartHeight,
                90.0f,
                this->lineHeight
            };

            Gdiplus::RectF actualLevelDisplay = {
                130.0f,
                this->dataStartHeight,
                30.0f,
                this->lineHeight
            };

            Gdiplus::RectF clearedLevelDisplay = {
                165.0f,
                this->dataStartHeight,
                30.0f,
                this->lineHeight
            };

            // Render all the possible levels in the hold
            for (
                unsigned int i = this->managedHold.holdParameters.maximum;
                i >= this->managedHold.holdParameters.minimum;
                i -= 1000
            ) {
                graphics.DrawString(
                    GetLevelDisplayString(i).c_str(),
                    3,
                    &this->font,
                    numbersDisplay,
                    &this->stringFormat,
                    &this->titleBarTextBrush
                );
                numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
            }

            // Loop the aircraft in the hold and render them
            for (
                ManagedHold::ManagedHoldAircraft::const_iterator it = this->managedHold.cbegin();
                it != this->managedHold.cend();
                ++it
            ) {                
                // The callsign display
                std::wstring callsign = ConvertToTchar(it->callsign);
                graphics.DrawString(
                    callsign.c_str(),
                    callsign.length(),
                    &this->font,
                    callsignDisplay,
                    &this->stringFormat,
                    &this->dataBrush
                );
                
                // Reported level
                graphics.DrawString(
                    GetLevelDisplayString(it->reportedLevel).c_str(),
                    3,
                    &this->font,
                    actualLevelDisplay,
                    &this->stringFormat,
                    &this->dataBrush
                );


                // Cleared level
                graphics.DrawString(
                    GetLevelDisplayString(it->clearedLevel).c_str(),
                    3,
                    &this->font,
                    actualLevelDisplay,
                    &this->stringFormat,
                    &this->dataBrush
                );

                unsigned int occupied = GetOccupiedLevel(it->reportedLevel, it->verticalSpeed);
                callsignDisplay.Y = this->dataStartHeight + (this->lineHeight * occupied);
                actualLevelDisplay.Y = this->dataStartHeight + (this->lineHeight * occupied);
                clearedLevelDisplay.Y = clearedLevelDisplay.Y + this->lineHeight;
            }

            // Border around whole thing, draw this last
            graphics.DrawRectangle(
                &this->borderPen,
                windowRect.left,
                windowRect.top,
                windowRect.right - windowRect.left - 1,
                windowRect.bottom - windowRect.top - 1
            );
        }

        /*
            The real callback used for the window messages
        */
        LRESULT HoldDisplay::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
                case WM_NCHITTEST: {
                    LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
                    Gdiplus::REAL xpos = (Gdiplus::REAL) GET_X_LPARAM(lParam);
                    Gdiplus::REAL ypos = (Gdiplus::REAL) GET_Y_LPARAM(lParam);
                    if (hit == HTCLIENT) hit = HTCAPTION;
                    return hit;
                }
                case WM_PAINT: {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    this->PaintWindow(hdc);
                    EndPaint(hwnd, &ps);
                    return TRUE;
                }
                case WM_CLOSE:
                    DestroyWindow(hwnd);
                    break;
                case WM_DESTROY:
                    break;
                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }
            return FALSE;
        }

        /*
            The callback used for the window messages - has to be static because its inherently C
        */
        LRESULT CALLBACK HoldDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_CREATE) {
                LogInfo("Hold manager window opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<LONG>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams)
                );
            }
            else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Hold manager window closed");
            }

            HoldDisplay * holdDisplay = reinterpret_cast<HoldDisplay*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return holdDisplay ? holdDisplay->_WndProc(hwnd, msg, wParam, lParam) :
                DefWindowProc(hwnd, msg, wParam, lParam);
        }

        /*
            Register ourselves with windows as a window class
        */
        void HoldDisplay::RegisterWindowClass(HINSTANCE dllInstance)
        {
            this->windowClass.cbSize = sizeof(this->windowClass);
            this->windowClass.lpfnWndProc = WndProc;
            this->windowClass.hInstance = dllInstance;
            this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            this->windowClass.hbrBackground = this->backgroundBrush;
            this->windowClass.lpszClassName = this->windowClassName;


            if (!RegisterClassEx(&this->windowClass)) {
                LogError("Unable to register hold display class");
                return;
            };

            this->windowRegistered = true;
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
