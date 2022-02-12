#pragma once
#include "setting/SettingRepositoryInterface.h"

namespace UKControllerPlugin::Setting {
    class SettingProviderInterface;

    class SettingRepository : public SettingRepositoryInterface
    {
        public:
        void AddProvider(std::shared_ptr<SettingProviderInterface> provider);
        [[nodiscard]] auto GetSetting(const std::string& setting, const std::string& defaultValue = "") const -> std::string override;
        [[nodiscard]] auto HasSetting(const std::string& setting) const -> bool override;
        void UpdateSetting(const std::string& setting, const std::string& value) override;
        void ReloadSetting(const std::string& setting) override;
        [[nodiscard]] auto CountSettings() const -> size_t;

        private:
        // Setting key to provider map
        std::map<std::string, std::shared_ptr<SettingProviderInterface>> settings;
    };
} // namespace UKControllerPlugin::Setting
