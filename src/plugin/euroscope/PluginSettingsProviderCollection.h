#pragma once

namespace UKControllerPlugin::Euroscope {
    class PluginSettingsProviderInterface;
    class UserSetting;

    /**
     * A collection of plugin settings providers
     */
    class PluginSettingsProviderCollection
    {
        public:
        explicit PluginSettingsProviderCollection(UserSetting& settings);
        void AddProvider(std::shared_ptr<PluginSettingsProviderInterface> provider);
        [[nodiscard]] auto Count() const -> size_t;
        void Load() const;
        void Save() const;

        private:
        
        // Settings
        UserSetting& settings;
        
        // Providers
        std::list<std::shared_ptr<PluginSettingsProviderInterface>> providers;
    };
} // namespace UKControllerPlugin::Euroscope
