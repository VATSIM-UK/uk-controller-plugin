#pragma once
#include "setting/SettingRepositoryInterface.h"

namespace UKControllerPlugin::Setting {
    class SettingProviderInterface;

    class SettingRepository : public SettingRepositoryInterface
    {
        public:
        void AddProvider(std::shared_ptr<SettingProviderInterface> provider);
        [[nodiscard]] auto GetSetting(std::string setting, std::string defaultValue = "") const -> std::string override;
        [[nodiscard]] auto HasSetting(std::string setting) const -> bool override;
        void UpdateSetting(std::string setting, std::string value) override;

        private:
        // Setting key to provider map
        std::map<std::string, std::shared_ptr<SettingProviderInterface>> settings;
    };
} // namespace UKControllerPlugin::Setting
