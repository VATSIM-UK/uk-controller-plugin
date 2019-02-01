            // Register the window class.
            WNDCLASSEX wc;
            HWND hwnd;
            MSG Msg;
            const LPCWSTR g_szClassName = L"myWindowClass";

            //Step 1: Registering the Window Class
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = 0;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = this->dllInstance;
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = g_szClassName;
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


            RegisterClassEx(&wc);
            // Create the window.
            hwnd = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                g_szClassName,
                L"The title of my window",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                NULL, NULL, this->dllInstance, NULL);
            ShowWindow(hwnd, 5);