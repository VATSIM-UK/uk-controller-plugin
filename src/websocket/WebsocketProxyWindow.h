#pragma once

namespace UKControllerPlugin {
    namespace Websocket {
        LRESULT CALLBACK WebsocketProxyWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        ULONG_PTR GetWebsocketProxyMessageIdentifier();
        std::wstring GetProxyWindowName();
    } // namespace Websocket
}  // namespace UKControllerPlugin
