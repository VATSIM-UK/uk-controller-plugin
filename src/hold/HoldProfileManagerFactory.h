#pragma once
#include "hold/HoldProfileManager.h"

namespace UKControllerPlugin {
    namespace Hold {

        std::unique_ptr<UKControllerPlugin::Hold::HoldProfileManager> CreateHoldProfileManager(nlohmann::json data);
    }  // namespace Hold
}  // namespace UKControllerPlugin
