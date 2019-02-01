#include "pch/stdafx.h"
#include "hold/HoldWindow.h"

namespace UKControllerPlugin {
    namespace Hold {

        const LPCWSTR HoldWindow::objectProp = L"HoldWindowObject";

        HoldWindow::HoldWindow(HWND euroscopeWindow, HINSTANCE dllInstance)
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance)
        {

        }

        /*
            Display the window
        */
        void HoldWindow::DisplayWindow(void)
        {
            if (!this->windowRegistered) {
                RegisterWindowClass();
            }

            this->selfHandle = CreateWindowEx(
                WS_EX_TOPMOST,
                windowClassName,
                L"UKCP Hold Manager",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 600,
                NULL, NULL, this->dllInstance, this
            );
            DWORD err = GetLastError();

            if (!this->selfHandle) {
                LogError("Unable to initialise the hold manager window");
            }

            ShowWindow(this->selfHandle, SW_SHOW);
        }

        /*
            The real callback used for the window messages
        */
        LRESULT HoldWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            return 0;
        }

        /*
            The callback used for the window messages - has to be static because its inherently C
        */
        LRESULT CALLBACK WndProcTest(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            return 0;
        }

        /*
            The callback used for the window messages - has to be static because its inherently C
        */
        LRESULT CALLBACK HoldWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            HANDLE objectProp = GetProp(hwnd, HoldWindow::objectProp);
            if (!objectProp) {
                SetProp(hwnd, HoldWindow::objectProp, (HANDLE) lParam);
            }

            if (msg == WM_INITDIALOG) {
                LogInfo("Hold manager window opened");
            } else if (msg == WM_NCDESTROY) {
                RemoveProp(hwnd, HoldWindow::objectProp);
                LogInfo("Hold manager window closed");
            }

            HoldWindow * pThis = (HoldWindow *) objectProp;
            return pThis ? pThis->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Register ourselves with windows as a window class
        */
        void HoldWindow::RegisterWindowClass(void)
        {
            this->windowClass.cbSize = sizeof(this->windowClass);
            this->windowClass.style = 0;
            this->windowClass.lpfnWndProc = WndProc;
            this->windowClass.cbClsExtra = 0;
            this->windowClass.cbWndExtra = 0;
            this->windowClass.hInstance = this->dllInstance;
            this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            this->windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            this->windowClass.lpszMenuName = NULL;
            this->windowClass.lpszClassName = this->windowClassName;
            this->windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


            if (!RegisterClassEx(&this->windowClass)) {
                LogError("Unable to register hold manager window");
                return;
            };

            this->windowRegistered = true;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
