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
                    CreateStaticHoldInformation(hwnd, 10, 10);
                    this->profileSelector = CreateProfileSelector(hwnd);
                    BOOL test = UpdateWindow(hwnd);
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

        HWND HoldWindow::CreateHoldView(HWND hwnd, unsigned int x, unsigned int y)
        {
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_LISTVIEW_CLASSES;

            if (!InitCommonControlsEx(&icex)) {
                LogError("Unable to initialise common controls");
            }

            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            HWND hWndListView = CreateWindow(
                WC_LISTVIEW,
                L"",
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
                x, y,
                250,
                250,
                hwnd,
                NULL,
                (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL
            );

            WCHAR szText[256];
            LVCOLUMN lvc;
            int iCol;
            int colCount = 4;

            // Initialize the LVCOLUMN structure.
            // The mask specifies that the format, width, text,
            // and subitem members of the structure are valid.
            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

            // Add the columns.
            for (iCol = 0; iCol < colCount; iCol++)
            {
                lvc.iSubItem = iCol;
                lvc.pszText = szText;
                lvc.cx = iCol == 0 ? 100 : 50;               // Width of column in pixels.
                lvc.fmt = LVCFMT_LEFT;

                // Load the names of the column headings from the string resources.
                LoadString(
                    (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                    IDS_STRING_HOLD_CS + iCol,
                    szText,
                    sizeof(szText) / sizeof(szText[0])
                );

                // Insert the columns into the list view.
                if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1) {
                    return FALSE;
                }
            }

            LVITEM lvI;
            // Initialize LVITEM members that are common to all items.
            lvI.pszText = L"Test"; // Sends an LVN_GETDISPINFO message.
            lvI.mask = LVIF_TEXT | LVIF_STATE;
            lvI.stateMask = 0;
            lvI.state = 0;
            lvI.iSubItem = 0; // MUST be zero

            // Initialize LVITEM members that are different for each item.
            for (int index = 0; index < colCount; index++)
            {
                lvI.iItem = index;

                // Insert items into the list.
                if (ListView_InsertItem(hWndListView, &lvI) == -1) {
                    return FALSE;
                }

                ListView_SetItemText(hWndListView, index, 0, L"BAW1259");
                ListView_SetItemText(hWndListView, index, 1, L"84");
                ListView_SetItemText(hWndListView, index, 2, L"80");
                ListView_SetItemText(hWndListView, index, 3, L"5");
            }

            return hWndListView;
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

        HWND HoldWindow::CreateStatic(HWND hwnd)
        {
            return CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"TEST STATIC CODE",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
                200,         // x position 
                200,         // y position 
                100,        // Button width
                100,        // Button height
                hwnd,     // Parent window
                NULL,       // No menu.
                (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                NULL
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
