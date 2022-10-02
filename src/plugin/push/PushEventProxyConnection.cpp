#include "push/PushEventProxyConnection.h"

namespace UKControllerPlugin {
    namespace Push {

        PushEventProxyConnection::PushEventProxyConnection()
        {
            RegisterClass(&this->windowClass);

            this->hiddenWindow = CreateWindow(
                L"UKControllerPluginPushEventProxyClass",
                L"UKControllerPluginPushEventProxy",
                NULL,
                0,
                0,
                0,
                0,
                NULL,
                NULL,
                GetModuleHandle(NULL),
                this);

            if (this->hiddenWindow == nullptr) {
                LogError("Unable to create push event proxy");
            }
        }

        PushEventProxyConnection::~PushEventProxyConnection(void)
        {
            if (this->hiddenWindow != nullptr) {
                DestroyWindow(this->hiddenWindow);
                UnregisterClass(L"UKControllerPluginPushEventProxyClass", GetModuleHandle(nullptr));
            }
        }

        void PushEventProxyConnection::AddMessageToQueue(std::string message)
        {
            std::lock_guard lock(this->messageLock);
            this->messages.push(message);
            LogDebug("Recieved proxy push event message: " + message);
        }

        std::string PushEventProxyConnection::GetNextMessage()
        {
            std::lock_guard lock(this->messageLock);
            if (this->messages.empty()) {
                return this->noMessage;
            }

            std::string message = this->messages.front();
            this->messages.pop();
            return message;
        }
    } // namespace Push
} // namespace UKControllerPlugin
