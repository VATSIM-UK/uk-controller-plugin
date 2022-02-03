#pragma once

namespace UKControllerPlugin::Setting {
    /**
     * Interface for classes providing plugin settings
     */
    class SettingRepositoryInterface
    {
        public:
        virtual ~SettingRepositoryInterface() = default;
        [[nodiscard]] virtual auto GetSetting(std::string setting, std::string defaultValue = "") const
            -> std::string = 0;
        [[nodiscard]] virtual auto HasSetting(std::string setting) const -> bool = 0;
        virtual void UpdateSetting(std::string setting, std::string value) = 0;
    };
} // namespace UKControllerPlugin::Setting
