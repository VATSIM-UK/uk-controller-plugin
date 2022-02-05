#pragma once
#include "api/ApiInterface.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Api {
    class ApiFactory;

    [[nodiscard]] auto Bootstrap(
        UKControllerPlugin::Setting::SettingRepository& settingRepository,
        UKControllerPlugin::Windows::WinApiInterface& windows,
        UKControllerPlugin::Curl::CurlInterface& curl) -> std::shared_ptr<ApiFactory>;

    [[nodiscard]] auto BootstrapLegacy(ApiFactory& factory, UKControllerPlugin::Curl::CurlInterface& curl)
        -> std::unique_ptr<UKControllerPlugin::Api::ApiInterface>;
} // namespace UKControllerPluginUtils::Api
