#pragma once

namespace UKControllerPlugin::Setting {
    /**
     * Interface for classes providing plugin settings
     */
    class SettingRepositoryInterface
    {
        public:
        virtual ~SettingRepositoryInterface() = default;
        [[nodiscard]] virtual auto GetSetting(const std::string& setting, const std::string& defaultValue) const
            -> std::string = 0;
        [[nodiscard]] virtual auto HasSetting(const std::string& setting) const -> bool = 0;
        virtual void UpdateSetting(const std::string& setting, const std::string& value) = 0;
        virtual void ReloadSetting(const std::string& setting) = 0;
    };
} // namespace UKControllerPlugin::Setting
