#pragma once
#include "hold/HoldManager.h"

namespace UKControllerPlugin {
    namespace Hold {

        std::unique_ptr<UKControllerPlugin::Hold::HoldManager> CreateHoldManager(nlohmann::json data);
    }  // namespace Hold
}  // namespace UKControllerPlugin
