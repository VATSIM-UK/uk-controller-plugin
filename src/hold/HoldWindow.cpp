#include "pch/stdafx.h"
#include "hold/HoldWindow.h"

namespace UKControllerPlugin {
    namespace Hold {

        HoldWindow::HoldWindow(HWND euroscopeWindow, HINSTANCE dllInstance)
            : euroscopeWindow(euroscopeWindow), dllInstance(dllInstance)
        {
            // Register the window class.
            const LPCWSTR g_szClassName = L"HoldWindowClass";
            this->windowClass.cbSize = sizeof(WNDCLASSEX);
            this->windowClass.style = 0;
            this->windowClass.lpfnWndProc = &HoldWindow::WndProc;
            this->windowClass.cbClsExtra = 0;
            this->windowClass.cbWndExtra = 0;
            this->windowClass.hInstance = this->dllInstance;
            this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            this->windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            this->windowClass.lpszMenuName = NULL;
            this->windowClass.lpszClassName = g_szClassName;
            this->windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


            RegisterClassEx(&this->windowClass);

            // Create the window.
            this->selfHandle = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                g_szClassName,
                L"The title of my window",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 600,
                NULL, NULL, this->dllInstance, NULL
            );
        }

        /*
            The real callback used for the window messages
        */
        LRESULT CALLBACK HoldWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            if (msg == WM_INITDIALOG) {
                SetProp(hwnd, L"local_class", (HANDLE)lParam);
            } else if (msg == WM_NCDESTROY) {
                RemoveProp(hwnd, L"local_class");
            }

            HoldWindow * pThis = (HoldWindow *) GetProp(hwnd, L"local_class");
            
            return pThis ? pThis->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin

