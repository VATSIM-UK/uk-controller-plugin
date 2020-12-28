#include "pch/stdafx.h"
#include "integration/HiddenWindow.h"
#include "integration/ExternalMessageEventHandler.h"

namespace UKControllerPlugin {
    namespace Integration {
        ExternalMessageEventHandler* handler = nullptr;

        /*
         * When using COPYDATASTRUCT the dwData member is essentially an enum that dictates what kind of data is being
         * sent. It can be anything. For UKCP we use a single number as a check to ensure that the data is intended
         * for us and the handlers will parse the message to see what it actually is and process it.
         */
        const ULONG_PTR dataType = 811;

        LRESULT CALLBACK HiddenWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                case WM_CREATE: {
                    handler = reinterpret_cast<ExternalMessageEventHandler*>(
                        reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams
                    );
                    return TRUE;
                }
                case WM_COPYDATA: {
                    COPYDATASTRUCT* data = reinterpret_cast<COPYDATASTRUCT*>(lParam);

                    if (data != nullptr && data->dwData == dataType && data->lpData != nullptr && handler != nullptr) {
                        handler->AddMessageToQueue(reinterpret_cast<const char*>(data->lpData));
                    }
                    return TRUE;
                }
            }

            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin
