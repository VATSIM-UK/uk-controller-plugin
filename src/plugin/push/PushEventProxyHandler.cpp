#include "pch/pch.h"
#include "push/PushEventProxyHandler.h"
#include "push/PushEventProxyWindow.h"

namespace UKControllerPlugin::Push {

        /*
            Handle the ping messages by responding with pong.
        */
        void PushEventProxyHandler::ProcessPushEvent(const PushEvent& message)
        {
            // Proxy the push event to any proxies listening - in regular string
            COPYDATASTRUCT cds;
            cds.dwData = GetPushEventProxyMessageIdentifier();
            cds.cbData = message.raw.size() + 1;
            cds.lpData = std::get<PVOID>(std::variant<PVOID, const char*>(message.raw.c_str()));
            EnumWindows(
                EnumerateWindows,
                std::get<LPARAM>(std::variant<LPARAM, COPYDATASTRUCT*>(&cds))
            );
        }

        /*
            Subscribe to pusher ping messages.
        */
        auto PushEventProxyHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
        {
            return {
                {
                    PushEventSubscription::SUB_TYPE_ALL,
                }
            };
        }

        void PushEventProxyHandler::PluginEventsSynced() {}
        
        auto PushEventProxyHandler::EnumerateWindows(HWND hwnd, LPARAM lparam) -> BOOL
        {
            std::array<WCHAR, WINDOW_NAME_BUFFER_SIZE> windowName = {};
            GetWindowText(
                hwnd,
                std::get<LPWSTR>(std::variant<LPWSTR, std::array<WCHAR, WINDOW_NAME_BUFFER_SIZE>*>(&windowName)),
                WINDOW_NAME_BUFFER_SIZE
            );
            std::wstring windowNameWide(windowName.cbegin(), windowName.cend());

            if (windowNameWide == GetProxyWindowName()) {
                SendMessage(
                    hwnd,
                    WM_COPYDATA,
                    std::get<WPARAM>(std::variant<WPARAM, HWND>(hwnd)),
                    lparam
                );
            }

            return TRUE;
        }
    } // namespace UKControllerPlugin::Push
