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
            explicit MockExternalMessageHandlerInterface(bool processMessage);
            virtual ~MockExternalMessageHandlerInterface();
            bool ProcessMessage(std::string message);

            bool hasBeenCalled = false;

            const bool processMessage;

            std::string receivedMessage;
        };
    } // namespace Integration
} // namespace UKControllerPluginTest
