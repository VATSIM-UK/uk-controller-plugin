#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        bool HoldDisplay::windowRegistered = false;

        HoldDisplay::HoldDisplay(HWND euroscopeWindow, HINSTANCE dllInstance)
            : titleBarBrush(Gdiplus::Color(255, 153, 153)), backgroundBrush(CreateSolidBrush(RGB(0, 0, 0))),
            titleBarTextBrush(Gdiplus::Color(255, 255, 255)), fontFamily(L"EuroScope"),
            font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip),
            dataBrush(Gdiplus::Color(0, 176, 0)),
            clearedLevelBrush(Gdiplus::Color(255, 128, 64)),
            borderPen(Gdiplus::Color(255, 255, 255), 1.5f),
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255))
        {
         
            if (!this->windowRegistered) {
                RegisterWindowClass(dllInstance);
            }

            this->stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

            this->selfHandle = CreateWindowEx(
                WS_EX_TOPMOST,
                windowClassName,
                L"UKCP Hold Manager",
                NULL,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                225,
                250,
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

            for (int i = 0; i < 8; i++) {
                if (i == 4) {
                    graphics.FillRectangle(
                        &this->blockedLevelBrush,
                        (INT) windowRect.left,
                        (INT) numbersDisplay.Y,
                        (INT) windowRect.right - windowRect.left,
                        (INT) this->lineHeight
                    );
                }

                graphics.DrawString(L"110", 3, &this->font, numbersDisplay, &this->stringFormat, &this->titleBarTextBrush);
                graphics.DrawString(L"BAW123", 6, &this->font, callsignDisplay, &this->stringFormat, &this->dataBrush);
                graphics.DrawString(L"083", 3, &this->font, actualLevelDisplay, &this->stringFormat, &this->dataBrush);
                graphics.DrawString(L"080", 3, &this->font, clearedLevelDisplay, &this->stringFormat, &this->clearedLevelBrush);

                numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
                callsignDisplay.Y = callsignDisplay.Y + this->lineHeight;
                actualLevelDisplay.Y = actualLevelDisplay.Y + this->lineHeight;
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
