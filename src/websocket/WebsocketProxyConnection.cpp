#include "pch/stdafx.h"
#include "websocket/WebsocketProxyConnection.h"

namespace UKControllerPlugin {
    namespace Websocket {

        WebsocketProxyConnection::WebsocketProxyConnection()
        {
            RegisterClass(&this->windowClass);

            this->hiddenWindow = CreateWindow(
                L"UKControllerPluginWebsocketProxyClass",
                L"UKControllerPluginWebsocketProxy",
                NULL,
                0,
                0,
                0,
                0,
                NULL,
                NULL,
                GetModuleHandle(NULL),
                reinterpret_cast<LPVOID>(this)
            );

            if (this->hiddenWindow == nullptr) {
                LogError("Unable to create websocket proxy");
            }
        }

        WebsocketProxyConnection::~WebsocketProxyConnection(void)
        {
            if (this->hiddenWindow != nullptr) {
                DestroyWindow(this->hiddenWindow);
                UnregisterClass(L"UKControllerPluginWebsocketProxyClass", GetModuleHandle(nullptr));
            }
        }

        void WebsocketProxyConnection::AddMessageToQueue(std::string message)
        {
            std::lock_guard<std::mutex> lock(this->messageLock);
            this->messages.push(message);
            LogDebug("Recieved proxy websocket message: " + message);
        }

        std::string WebsocketProxyConnection::GetNextMessage()
        {
            std::lock_guard<std::mutex> lock(this->messageLock);
            if (this->messages.empty()) {
                return this->noMessage;
            }

            std::string message = this->messages.front();
            this->messages.pop();
            return message;
        }
    } // namespace Websocket
} // namespace UKControllerPlugin
