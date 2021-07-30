#pragma once
#include "push/PushEventConnectionInterface.h"
#include "push/PushEventProxyWindow.h"

namespace UKControllerPlugin {
    namespace Push {
        /*
            Receives messages from external sources and passes them on.
        */
        class PushEventProxyConnection : public PushEventConnectionInterface
        {
            public:
                PushEventProxyConnection();
                ~PushEventProxyConnection() override;

                void AddMessageToQueue(std::string message);
                void WriteMessage(std::string message) override {} // We dont need to implement this
                std::string GetNextMessage() override;

            private:
                // Lock for the message queue
                std::mutex messageLock;

                // Internal message quque
                std::queue<std::string> messages;

                // The hidden window handle
                HWND hiddenWindow = nullptr;

                // Hidden window
                WNDCLASS windowClass = {
                    NULL,
                    PushEventProxyWindow,
                    NULL,
                    NULL,
                    GetModuleHandle(nullptr),
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    L"UKControllerPluginPushEventProxyClass"
                };

                // The command we should receive if a new message is sent
                std::string newMessageCommand = ".ukcp msg ";
        };
    } // namespace Push
} // namespace UKControllerPlugin
