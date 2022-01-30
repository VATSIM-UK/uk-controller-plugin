#pragma once

namespace UKControllerPlugin::Setting {
    class SettingRepository;
} // namespace UKControllerPlugin::Setting

namespace UKControllerPluginUtils::Api {
    class ApiSettings;

    [[nodiscard]] auto Make(const UKControllerPlugin::Setting::SettingRepository& settings)
        -> std::shared_ptr<ApiSettings>;
} // namespace UKControllerPluginUtils::Api
