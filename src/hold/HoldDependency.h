#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {
        void UpdateHoldDependency(
            const UKControllerPlugin::Api::ApiInterface & api,
            UKControllerPlugin::Windows::WinApiInterface & winApi
        );
        nlohmann::json GetLocalHoldData(UKControllerPlugin::Windows::WinApiInterface & winApi);
    }  // namespace Hold
}  // namespace UKControllerPlugin
