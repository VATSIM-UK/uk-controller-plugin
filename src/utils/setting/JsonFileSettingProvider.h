#pragma once
#include "SettingProviderInterface.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Setting {
    /**
     * Provides settings from a JSON object stored in a file.
     */
    class JsonFileSettingProvider : public SettingProviderInterface
    {
        public:
        JsonFileSettingProvider(
            const std::wstring filename, std::set<std::string> providedSettings, Windows::WinApiInterface& windows);
        auto Get(const std::string& key) -> std::string override;
        void Save(const std::string& key, const std::string& value) override;
        auto Provides() -> const std::set<std::string>& override;
        void Reload() override;

        private:
        [[nodiscard]] auto LoadFromFile() const -> std::map<std::string, std::string>;

        // The filename to load
        const std::wstring filename;

        // The settings provided in this config file
        const std::set<std::string> providedSettings;

        // Windows API for loading files
        Windows::WinApiInterface& windows;

        // Loaded settings
        std::map<std::string, std::string> loadedSettings;
    };
} // namespace UKControllerPlugin::Setting
