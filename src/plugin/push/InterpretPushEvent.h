#pragma once
#include "push/PushEvent.h"

namespace UKControllerPlugin::Push {
    const PushEvent invalidMessage = {"error_invalid", "error_invalid"};

    PushEvent InterpretPushedEvent(std::string message);
} // namespace UKControllerPlugin::Push
