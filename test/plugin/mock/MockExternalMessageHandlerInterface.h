#pragma once
#include "integration/ExternalMessageHandlerInterface.h"

namespace UKControllerPluginTest {
    namespace Integration {
        /*
            Interfaces for classes that want to process messages from external sources.
        */
        class MockExternalMessageHandlerInterface
            : public UKControllerPlugin::Integration::ExternalMessageHandlerInterface
        {
            public:
            explicit MockExternalMessageHandlerInterface(bool processMessage) : processMessage(processMessage)
            {
            }

            bool ProcessMessage(std::string message)
            {
                this->hasBeenCalled = true;
                this->receivedMessage = message;
                return processMessage;
            };

            bool hasBeenCalled = false;

            const bool processMessage;

            std::string receivedMessage;
        };
    } // namespace Integration
} // namespace UKControllerPluginTest
