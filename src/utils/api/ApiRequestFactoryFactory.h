#pragma once

namespace UKControllerPlugin {
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Api {
    class ApiRequestFactory;

    [[nodiscard]] auto
    Make(const UKControllerPlugin::Setting::SettingRepository& settings, UKControllerPlugin::Curl::CurlInterface& curl)
        -> std::unique_ptr<ApiRequestFactory>;
} // namespace UKControllerPluginUtils::Api
