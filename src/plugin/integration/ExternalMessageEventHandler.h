#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "integration/HiddenWindow.h"
#include "integration/ExternalMessageHandlerInterface.h"
#include "command/CommandHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Integration {
        class MessageInterface;

        /*
            Receives messages from external sources and passes them on.
        */
        class ExternalMessageEventHandler : public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
                                            public UKControllerPlugin::Command::CommandHandlerInterface
        {
            public:
            explicit ExternalMessageEventHandler(bool duplicatePlugin);
            ~ExternalMessageEventHandler() override;
            void AddHandler(std::shared_ptr<ExternalMessageHandlerInterface> handler);
            void AddMessageToQueue(std::string message);
            size_t CountHandlers(void) const;

            // Inherited via AbstractTimedEvent
            void TimedEventTrigger(void) override;

            // Inherited via CommandHandlerInterface
            bool ProcessCommand(std::string command) override;

            private:
            std::set<std::shared_ptr<ExternalMessageHandlerInterface>> eventHandlers;

            // Lock for the message queue
            std::mutex messageLock;

            // Internal message queue
            std::queue<std::string> messages;

            // The hidden window handle
            HWND hiddenWindow = NULL;

            // Hidden window
            WNDCLASS windowClass = {
                NULL,
                HiddenWindow,
                NULL,
                NULL,
                GetModuleHandle(NULL),
                NULL,
                NULL,
                NULL,
                NULL,
                L"UKControllerPluginHiddenWindowClass"};

            // The command we should receive if a new message is sent
            std::string newMessageCommand = ".ukcp msg ";
        };
    } // namespace Integration
} // namespace UKControllerPlugin
