#pragma once
#include "wake/WakeCategoryMapper.h"

namespace UKControllerPlugin {
    namespace Message {
        class UserMessager;
    }  // namespace Message
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Wake {
        WakeCategoryMapper CreateWakeMappings(
            nlohmann::json jsonData,
            UKControllerPlugin::Message::UserMessager & messager
        );
    }  // namespace Wake
}  // namespace UKControllerPlugin
