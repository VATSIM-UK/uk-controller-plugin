#include "MockExternalMessageHandlerInterface.h"

namespace UKControllerPluginTest::Integration {
    MockExternalMessageHandlerInterface::MockExternalMessageHandlerInterface(bool processMessage)
        : processMessage(processMessage)
    {
    }

    MockExternalMessageHandlerInterface::~MockExternalMessageHandlerInterface() = default;

    auto MockExternalMessageHandlerInterface::ProcessMessage(std::string message) -> bool
    {
        this->hasBeenCalled = true;
        this->receivedMessage = message;
        return processMessage;
    }
} // namespace UKControllerPluginTest::Integration
