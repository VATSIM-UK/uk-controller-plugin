#include "SettingProviderInterface.h"
#include "SettingRepository.h"

namespace UKControllerPlugin::Setting {

    void SettingRepository::AddProvider(std::shared_ptr<SettingProviderInterface> provider)
    {
        for (const auto& setting : provider->Provides()) {
            if (this->settings.count(setting)) {
                LogWarning("Detected second provider for setting " + setting);
                continue;
            }

            this->settings[setting] = provider;
        }
    }

    auto SettingRepository::GetSetting(std::string setting, std::string defaultValue) const -> std::string
    {
        if (!this->HasSetting(setting)) {
            return defaultValue;
        }

        auto value = this->settings.at(setting)->Get(setting);
        return value.empty() ? defaultValue : value;
    }

    auto SettingRepository::HasSetting(std::string setting) const -> bool
    {
        return this->settings.count(setting) > 0;
    }

    void SettingRepository::UpdateSetting(std::string setting, std::string value)
    {
        if (!this->HasSetting(setting)) {
            return;
        }

        this->settings.at(setting)->Save(setting, value);
    }
} // namespace UKControllerPlugin::Setting
