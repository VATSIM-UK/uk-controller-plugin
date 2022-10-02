#pragma once

namespace UKControllerPlugin {
    namespace Push {
        LRESULT CALLBACK PushEventProxyWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        ULONG_PTR GetPushEventProxyMessageIdentifier();
        std::wstring GetProxyWindowName();
    } // namespace Push
} // namespace UKControllerPlugin
