#include "pch/stdafx.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        HWND CreateProfileSelector(HWND parent)
        {
            RECT parentArea;
            GetClientRect(parent, &parentArea);
            HoldElementDimensions dimensions = GetProfileSelectorDimensions(parentArea);

            HWND wnd = CreateWindow(
                WC_COMBOBOX,
                TEXT(""),
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                dimensions.topLeftX,
                dimensions.topLeftY,
                dimensions.width,
                dimensions.height,
                parent,
                NULL,
                (HINSTANCE) GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );

            // Add test strings
            SendMessage(wnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM) L"Test value 1");
            SendMessage(wnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM) L"Test value 2");
            SendMessage(wnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM) L"Test value 3");

            // Select one
            SendMessage(wnd, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

            return wnd;
        }

        /*
            Converts a C++ string to TCHAR so that we can use it in displays.
        */
        TCHAR * ConvertToTchar(std::string string)
        {
            TCHAR * param = new TCHAR[string.size() + 1];
            param[string.size()] = 0;
            std::copy(string.begin(), string.end(), param);
            return param;
        }

        /*
            Convert an integer to string then to TCHAR
        */
        TCHAR * ConvertToTchar(int value)
        {
            return ConvertToTchar(std::to_string(value));
        }

        /*
            Returns the dimensions of the profile selector based on its
            parent
        */
        HoldElementDimensions GetProfileSelectorDimensions(RECT parent)
        {
            const int selectorWidth = 200;
            const int selectorHeight = 300;

            return HoldElementDimensions {
                ((parent.right - parent.left) / 2) - (selectorWidth / 2),
                parent.bottom - 50,
                selectorWidth,
                selectorHeight
            };
        }

        /*
            Moves the profile selector based on its parent
        */
        void MoveProfileSelector(HWND handle, HWND parent)
        {
            RECT parentArea;
            GetClientRect(parent, &parentArea);
            HoldElementDimensions dimensions = GetProfileSelectorDimensions(parentArea);
            MoveWindow(handle, dimensions.topLeftX, dimensions.topLeftY, dimensions.width, dimensions.height, TRUE);
            UpdateWindow(parent);
        }

        void CreateStaticHoldInformation(HWND parent, unsigned int topLeftX, unsigned int topLeftY)
        {
            const int height = 20;
            const int width = 100;

            CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"TIMBA",      // Button text 
                WS_VISIBLE | WS_CHILD,  // Styles 
                topLeftX,         // x position 
                topLeftY,         // y position 
                width,        // Button width
                height,        // Button height
                parent,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );

            CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"Inbound: 309",      // Button text 
                WS_VISIBLE | WS_CHILD,  // Styles 
                topLeftX,         // x position 
                topLeftY + height,         // y position 
                width,        // Button width
                height,        // Button height
                parent,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );

            CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"Min: 7000",      // Button text 
                WS_VISIBLE | WS_CHILD,  // Styles 
                topLeftX,         // x position 
                topLeftY + (height * 2),         // y position 
                width,        // Button width
                height,        // Button height
                parent,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );

            CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"Max: 15000",      // Button text 
                WS_VISIBLE | WS_CHILD,  // Styles 
                topLeftX,         // x position 
                topLeftY + (height * 3),         // y position 
                width,        // Button width
                height,        // Button height
                parent,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );

            CreateWindow(
                L"STATIC",  // Predefined class; Unicode assumed 
                L"Turn: Right",      // Button text 
                WS_VISIBLE | WS_CHILD,  // Styles 
                topLeftX,         // x position 
                topLeftY + (height * 4),         // y position 
                width,        // Button width
                height,        // Button height
                parent,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
                NULL
            );
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
