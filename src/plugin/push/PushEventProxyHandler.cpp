#include "PushEventProxyHandler.h"
#include "PushEventProxyWindow.h"

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
        cds.lpData = reinterpret_cast<PVOID>(reinterpret_cast<LPARAM>(message.raw.c_str())); // NOLINT
        EnumWindows(EnumerateWindows, reinterpret_cast<LPARAM>(&cds));                       // NOLINT
    }

    /*
        Subscribe to pusher ping messages.
    */
    auto PushEventProxyHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{
            PushEventSubscription::SUB_TYPE_ALL,
        }};
    }

    void PushEventProxyHandler::PluginEventsSynced()
    {
    }

    auto PushEventProxyHandler::EnumerateWindows(HWND hwnd, LPARAM lparam) -> BOOL
    {
        std::array<WCHAR, WINDOW_NAME_BUFFER_SIZE> windowName = {};
        int nameLength = GetWindowText(hwnd, reinterpret_cast<LPWSTR>(&windowName), WINDOW_NAME_BUFFER_SIZE); // NOLINT
        if (nameLength == 0) {
            return TRUE;
        }

        if (std::wstring(windowName.cbegin(), windowName.cbegin() + nameLength) == GetProxyWindowName()) {
            SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(hwnd), lparam); // NOLINT
        }

        return TRUE;
    }
} // namespace UKControllerPlugin::Push
