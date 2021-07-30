#include "pch/pch.h"
#include "push/PushEventProxyHandler.h"
#include "push/PushEventProxyWindow.h"

namespace UKControllerPlugin {
    namespace Push {

        /*
            Handle the ping messages by responding with pong.
        */
        void PushEventProxyHandler::ProcessPushEvent(const PushEvent& message)
        {
            // Proxy the push event to any proxies listening - in regular string
            COPYDATASTRUCT cds;
            cds.dwData = GetPushEventProxyMessageIdentifier();
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
        std::set<PushEventSubscription> PushEventProxyHandler::GetPushEventSubscriptions(void) const
        {
            return {
                {
                    PushEventSubscription::SUB_TYPE_ALL,
                }
            };
        }

        void PushEventProxyHandler::PluginEventsSynced() {}
    } // namespace Push
} // namespace UKControllerPlugin
