#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        bool HoldDisplay::windowRegistered = false;

        HoldDisplay::HoldDisplay(HWND euroscopeWindow, HINSTANCE dllInstance)
        {
         
            if (!this->windowRegistered) {
                RegisterWindowClass(dllInstance);
            }

            this->selfHandle = CreateWindowEx(
                WS_EX_TOPMOST,
                windowClassName,
                L"UKCP Hold Manager",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 600,
                euroscopeWindow, NULL, dllInstance, (LPVOID)this
            );

            if (!this->selfHandle) {
                LogError("Unable to create hold display");
                return;
            }

            ShowWindow(this->selfHandle, 5);
        }

        HoldDisplay::~HoldDisplay()
        {
            if (this->selfHandle) {
                DeleteObject(this->selfHandle);
            }
        }

                /*
            The real callback used for the window messages
        */
        LRESULT HoldDisplay::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
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
            this->windowClass.style = 0;
            this->windowClass.lpfnWndProc = WndProc;
            this->windowClass.cbClsExtra = 0;
            this->windowClass.cbWndExtra = 0;
            this->windowClass.hInstance = dllInstance;
            this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            this->windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            this->windowClass.lpszMenuName = NULL;
            this->windowClass.lpszClassName = this->windowClassName;
            this->windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


            if (!RegisterClassEx(&this->windowClass)) {
                LogError("Unable to register hold display class");
                return;
            };

            this->windowRegistered = true;
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
