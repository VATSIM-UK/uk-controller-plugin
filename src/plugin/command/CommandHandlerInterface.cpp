#include "CommandHandlerInterface.h"

namespace UKControllerPlugin::Command {
    CommandHandlerInterface::CommandHandlerInterface() = default;
    CommandHandlerInterface::~CommandHandlerInterface() = default;
    CommandHandlerInterface::CommandHandlerInterface(const CommandHandlerInterface&) = default;
    auto CommandHandlerInterface::operator=(const CommandHandlerInterface&) -> CommandHandlerInterface& = default;
} // namespace UKControllerPlugin::Command
