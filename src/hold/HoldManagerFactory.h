#pragma once
#include "hold/HoldManager.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Hold {

        std::unique_ptr<UKControllerPlugin::Hold::HoldManager> CreateHoldManager(
            nlohmann::json data,
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container
        );
    }  // namespace Hold
}  // namespace UKControllerPlugin
