#include "pch/stdafx.h"
#include "integration/HiddenWindow.h"
#include "integration/ExternalMessageEventHandler.h"

namespace UKControllerPlugin {
    namespace Integration {
        ExternalMessageEventHandler* handler = nullptr;

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

                    if (data != nullptr && data->dwData == 811 && data->lpData != nullptr && handler != nullptr) {
                        handler->AddMessageToQueue(reinterpret_cast<const char*>(data->lpData));
                    }
                    return TRUE;
                }
            }

            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin