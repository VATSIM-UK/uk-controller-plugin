#include "ExternalMessageHandlerInterface.h"

namespace UKControllerPlugin::Integration {
    ExternalMessageHandlerInterface::ExternalMessageHandlerInterface() = default;
    ExternalMessageHandlerInterface::~ExternalMessageHandlerInterface() = default;
    ExternalMessageHandlerInterface::ExternalMessageHandlerInterface(const ExternalMessageHandlerInterface&) = default;
    auto ExternalMessageHandlerInterface::operator=(const ExternalMessageHandlerInterface&)
        -> ExternalMessageHandlerInterface& = default;
} // namespace UKControllerPlugin::Integration
