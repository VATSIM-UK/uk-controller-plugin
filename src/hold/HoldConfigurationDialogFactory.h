#pragma once
#include "hold/HoldConfigurationDialog.h"

namespace UKControllerPlugin {
    namespace Hold {

        std::shared_ptr<UKControllerPlugin::Hold::HoldConfigurationDialog> CreateHoldConfigurationDialog(
            nlohmann::json data
        );
    }  // namespace Hold
}  // namespace UKControllerPlugin
