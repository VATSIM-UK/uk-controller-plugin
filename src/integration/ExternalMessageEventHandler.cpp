#include "pch/stdafx.h"
#include "integration/ExternalMessageEventHandler.h"
#include "integration//ExternalMessageHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Integration {

        ExternalMessageEventHandler::ExternalMessageEventHandler(bool duplicatePlugin)
        {
            if (duplicatePlugin)
            {
                return;
            }

            RegisterClass(&this->windowClass);

            this->hiddenWindow = CreateWindow(
                L"UKControllerPluginHiddenWindowClass",
                L"UKControllerPluginHiddenWindow",
                NULL,
                0,
                0,
                0,
                0,
                HWND_MESSAGE,
                NULL,
                GetModuleHandle(NULL),
                reinterpret_cast<LPVOID>(this)
            );

            if (GetLastError() != S_OK) {
                LogError("Unable to create external message handler");
            }
        }

        ExternalMessageEventHandler::~ExternalMessageEventHandler(void)
        {
            if (this->hiddenWindow != NULL) {
                DestroyWindow(this->hiddenWindow);
            }
        }

        void ExternalMessageEventHandler::AddHandler(std::shared_ptr<ExternalMessageHandlerInterface> handler)
        {
            if (!this->eventHandlers.insert(handler).second) {
                LogWarning("Duplicate external message event handler added");
            }
        }

        void ExternalMessageEventHandler::AddMessageToQueue(std::string message)
        {
            std::lock_guard<std::mutex> lock(this->messageLock);
            this->messages.push(message);
        }

        size_t ExternalMessageEventHandler::CountHandlers(void) const
        {
            return this->eventHandlers.size();
        }

        void ExternalMessageEventHandler::TimedEventTrigger(void)
        {
            std::lock_guard<std::mutex> lock(this->messageLock);

            // Process any incoming messages
            while (this->messages.size() != 0) {
                for (
                    auto handlerIt = this->eventHandlers.cbegin();
                    handlerIt != this->eventHandlers.cend();
                    ++handlerIt
                ) {
                    if ((*handlerIt)->ProcessMessage(this->messages.front())) {
                        break;
                    }

                }
                LogWarning("Unable to handle external message: " + this->messages.front());
                this->messages.pop();
            }
        }

        bool ExternalMessageEventHandler::ProcessCommand(std::string command)
        {
            if (command.substr(0, this->newMessageCommand.size()) != this->newMessageCommand) {
                return false;
            }

            #ifdef _DEBUG

            command = command.substr(this->newMessageCommand.size());

            COPYDATASTRUCT cds;
            cds.dwData = 811;
            cds.cbData = command.size() + 1;
            cds.lpData = (PVOID)command.c_str();

            // Find the hidden window
            HWND window = FindWindowEx(HWND_MESSAGE, NULL, this->windowClass.lpszClassName, NULL);
            if (window == NULL) {
                return true;
            }

            // Send the data
            SendMessage(window, WM_COPYDATA, reinterpret_cast<WPARAM>(window), reinterpret_cast<LPARAM>(&cds));
            #endif  // _DEBUG

            return true;
        }
    }  // namespace Integration
}  // namespace UKControllerPlugin
