#include "pch/stdafx.h"
#include "websocket/WebsocketProxyHandler.h"
#include "websocket/WebsocketProxyWindow.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Handle the ping messages by responding with pong.
        */
        void WebsocketProxyHandler::ProcessWebsocketMessage(const WebsocketMessage& message)
        {
            // Proxy the websocket message to any proxies listening - in regular string
            COPYDATASTRUCT cds;
            cds.dwData = GetWebsocketProxyMessageIdentifier();
            cds.cbData = message.raw.size() + 1;
            cds.lpData = reinterpret_cast<PVOID>(reinterpret_cast<LPARAM>(message.raw.c_str()));
            EnumWindows(
                [](HWND hwnd, LPARAM lparam) -> BOOL
                {
                    WCHAR windowName[1000];
                    GetWindowText(hwnd, reinterpret_cast<LPWSTR>(&windowName), 1000);
                    std::wstring windowNameWide(windowName);

                    if (windowNameWide == GetProxyWindowName()) {

                        SendMessage(
                            hwnd,
                            WM_COPYDATA,
                            reinterpret_cast<WPARAM>(hwnd),
                            lparam
                        );
                    }

                    return TRUE;
                },
                reinterpret_cast<LPARAM>(&cds)
            );
        }

        /*
            Subscribe to pusher ping messages.
        */
        std::set<WebsocketSubscription> WebsocketProxyHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_ALL,
                }
            };
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
