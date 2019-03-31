#pragma once
#include "hold/HoldConfigurationDialog.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        std::shared_ptr<UKControllerPlugin::Hold::HoldConfigurationDialog> CreateHoldConfigurationDialog(
            nlohmann::json data,
            UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager
        );
    }  // namespace Hold
}  // namespace UKControllerPlugin
