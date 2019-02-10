#pragma once
#include "hold/HoldManager.h"

namespace UKControllerPlugin {
    namespace Hold {

        std::unique_ptr<UKControllerPlugin::Hold::HoldManager> BuildHoldingData(nlohmann::json data);
        bool CheckValid(nlohmann::json data);

    }  // namespace Hold
}  // namespace UKControllerPlugin