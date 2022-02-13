#pragma once

namespace UKControllerPlugin::Setting {
    class SettingRepositoryInterface;
} // namespace UKControllerPlugin::Setting

namespace UKControllerPluginUtils::Api {
    class ApiSettings;

    [[nodiscard]] auto Make(const UKControllerPlugin::Setting::SettingRepositoryInterface& settings)
        -> std::shared_ptr<ApiSettings>;
} // namespace UKControllerPluginUtils::Api
