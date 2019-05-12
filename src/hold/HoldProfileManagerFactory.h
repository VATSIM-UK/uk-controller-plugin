#pragma once
#include "hold/HoldProfileManager.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        std::unique_ptr<UKControllerPlugin::Hold::HoldProfileManager> CreateHoldProfileManager(
            nlohmann::json data,
            const UKControllerPlugin::Api::ApiInterface & api
        );
    }  // namespace Hold
}  // namespace UKControllerPlugin
