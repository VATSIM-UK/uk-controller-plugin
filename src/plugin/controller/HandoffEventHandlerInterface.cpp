#include "HandoffEventHandlerInterface.h"

namespace UKControllerPlugin::Controller {
    HandoffEventHandlerInterface::HandoffEventHandlerInterface() = default;
    HandoffEventHandlerInterface::~HandoffEventHandlerInterface() = default;
    HandoffEventHandlerInterface::HandoffEventHandlerInterface(const HandoffEventHandlerInterface&) = default;
    auto HandoffEventHandlerInterface::operator=(const HandoffEventHandlerInterface&)
        -> HandoffEventHandlerInterface& = default;
} // namespace UKControllerPlugin::Controller
