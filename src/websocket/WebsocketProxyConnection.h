#pragma once
#include "websocket/WebsocketConnectionInterface.h"
#include "websocket/WebsocketProxyWindow.h"

namespace UKControllerPlugin {
    namespace Websocket {
        /*
            Receives messages from external sources and passes them on.
        */
        class WebsocketProxyConnection : public WebsocketConnectionInterface
        {
            public:
                WebsocketProxyConnection();
                ~WebsocketProxyConnection() override;

                void AddMessageToQueue(std::string message);
                void WriteMessage(std::string message) override {} // We dont need to implement this
                std::string GetNextMessage() override;
                void SetIdleTimeout(std::chrono::seconds timeout) override {} // We dont need to implement this

            private:

                // Lock for the message queue
                std::mutex messageLock;

                // Internal message quque
                std::queue<std::string> messages;

                // The hidden window handle
                HWND hiddenWindow = NULL;

                // Hidden window
                WNDCLASS windowClass = {
                   NULL,
                   WebsocketProxyWindow,
                   NULL,
                   NULL,
                   GetModuleHandle(NULL),
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   L"UKControllerPluginWebsocketProxyClass"
                };

                // The command we should receive if a new message is sent
                std::string newMessageCommand = ".ukcp msg ";
        };
    } // namespace Websocket
}  // namespace UKControllerPlugin
