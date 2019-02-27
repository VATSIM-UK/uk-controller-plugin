#pragma once
#include "hold/HoldConfigurationDialog.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldWindowManager;
        class HoldSelectionMenu;
        class HoldProfileManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        std::shared_ptr<UKControllerPlugin::Hold::HoldConfigurationDialog> CreateHoldConfigurationDialog(
            nlohmann::json data,
            UKControllerPlugin::Hold::HoldWindowManager & windowManager,
            UKControllerPlugin::Hold::HoldSelectionMenu & holdSelectionMenu,
            UKControllerPlugin::Hold::HoldProfileManager & holdProfileManager
        );
    }  // namespace Hold
}  // namespace UKControllerPlugin
