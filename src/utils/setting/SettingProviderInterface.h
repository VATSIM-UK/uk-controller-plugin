#pragma once

namespace UKControllerPlugin::Setting {

    /**
     * Provides an interface for classes that provide plugin settings.
     */
    class SettingProviderInterface
    {
        public:
        virtual ~SettingProviderInterface() = default;
        
        /**
         * Returns settings values. Returns empty string if not found.
         */
        [[nodiscard]] virtual auto Get(const std::string& key) -> std::string = 0;
        virtual void Save(const std::string& key, const std::string& value) = 0;
        [[nodiscard]] virtual auto Provides() -> const std::set<std::string>& = 0;
    };
} // namespace UKControllerPlugin::Setting
