#include "pch/stdafx.h"
#include "hold/HoldWindow.h"
#include "hold/HoldDisplayFunctions.h"

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
                NULL, NULL, this->dllInstance, (LPVOID) this
            );

            if (!this->selfHandle) {
                LogError("Unable to initialise the hold manager window");
            }

            ShowWindow(this->selfHandle, SW_SHOW);
            MSG Msg;
            while (GetMessage(&Msg, NULL, 0, 0) > 0) { 
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        }

        /*
            The real callback used for the window messages
        */
        LRESULT HoldWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
                case WM_CREATE: {
                    //HWND holdView = CreateHoldView(hwnd, 10, 100);
                    //HWND staticItem = CreateStatic(hwnd);
                    //CreateHoldInformation(hwnd, 10, 10);
                    //CreateStaticHoldInformation(hwnd, 10, 10);
                    this->profileSelector = CreateProfileSelector(hwnd);
                    this->holdDisplays.emplace_back(hwnd);
                    UpdateWindow(hwnd); 
                    return TRUE;
                }
                case WM_CLOSE:
                    DestroyWindow(hwnd);
                    break;
                case WM_DESTROY:
                    break;
                case WM_SIZE: {
                    MoveProfileSelector(this->profileSelector, hwnd);
                    return TRUE;
                }
                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }
            return FALSE;
        }

        /*
            The callback used for the window messages - has to be static because its inherently C
        */
        LRESULT CALLBACK HoldWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_CREATE) {
                LogInfo("Hold manager window opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<LONG>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams)
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("Hold manager window closed");
            }

            HoldWindow * holdWindow = reinterpret_cast<HoldWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return holdWindow ? holdWindow->_WndProc(hwnd, msg, wParam, lParam) : 
                DefWindowProc(hwnd, msg, wParam, lParam);
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

        HWND HoldWindow::CreateButton(HWND hwnd)
        {
            return CreateWindow(
                L"BUTTON",  // Predefined class; Unicode assumed 
                L"OK",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                10,         // x position 
                10,         // y position 
                100,        // Button width
                100,        // Button height
                hwnd,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL);      // Pointer not needed.
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
