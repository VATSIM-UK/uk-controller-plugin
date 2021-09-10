#include "AsrEventHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    AsrEventHandlerInterface::AsrEventHandlerInterface() = default;
    AsrEventHandlerInterface::~AsrEventHandlerInterface() = default;
    AsrEventHandlerInterface::AsrEventHandlerInterface(const AsrEventHandlerInterface&) = default;
    auto AsrEventHandlerInterface::operator=(const AsrEventHandlerInterface&) -> AsrEventHandlerInterface& = default;
} // namespace UKControllerPlugin::Euroscope
