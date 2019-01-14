#pragma once
#include "HoldingDataCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        UKControllerPlugin::Hold::HoldingDataCollection BuildHoldingData(nlohmann::json data);
        bool CheckValid(nlohmann::json data);

    }  // namespace Hold
}  // namespace UKControllerPlugin