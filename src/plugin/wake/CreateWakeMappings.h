#pragma once
#include "wake/WakeCategoryMapper.h"

namespace UKControllerPlugin::Message {
    class UserMessager;
} // namespace UKControllerPlugin::Message

namespace UKControllerPlugin::Wake {
    [[nodiscard]] auto CreateWakeMappings(
        const nlohmann::json& jsonData, UKControllerPlugin::Message::UserMessager& messager, std::string type = "wake")
        -> WakeCategoryMapper;
} // namespace UKControllerPlugin::Wake
