#pragma once
#include "push/PushEvent.h"

const UKControllerPlugin::Push::PushEvent invalidMessage = {"error_invalid", "error_invalid"};

UKControllerPlugin::Push::PushEvent InterpretPushedEvent(std::string message);
