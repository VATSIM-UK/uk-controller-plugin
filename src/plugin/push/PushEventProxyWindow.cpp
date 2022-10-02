#include "push/PushEventProxyWindow.h"
#include "push/PushEventProxyConnection.h"

namespace UKControllerPlugin {
    namespace Push {
        PushEventProxyConnection* handler = nullptr;

        LRESULT CALLBACK PushEventProxyWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_CREATE) {
                handler =
                    static_cast<PushEventProxyConnection*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
                return TRUE;
            }

            if (msg == WM_COPYDATA) {
                COPYDATASTRUCT* data = reinterpret_cast<COPYDATASTRUCT*>(lParam);

                if (data != nullptr && data->dwData == GetPushEventProxyMessageIdentifier() &&
                    data->lpData != nullptr && handler != nullptr) {
                    handler->AddMessageToQueue(reinterpret_cast<const char*>(data->lpData));
                }
                return TRUE;
            }

            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        /*
         * We have to have an identifier for these types of messages so we can disambiguate against
         * other messages passing through.
         */
        ULONG_PTR GetPushEventProxyMessageIdentifier()
        {
            return 2414;
        }

        std::wstring GetProxyWindowName()
        {
            return L"UKControllerPluginPushEventProxy";
        }
    } // namespace Push
} // namespace UKControllerPlugin
