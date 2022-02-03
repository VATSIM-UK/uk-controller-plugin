#pragma once

namespace UKControllerPluginUtils::Api {
    class ApiSettings;

    class ApiSettingsProviderInterface
    {
        public:
        
        virtual ~ApiSettingsProviderInterface() = default;
        /**
         * Save to storage whatever the provided settings are.
         */
        virtual void SaveSettings(const ApiSettings& settings) = 0;

        /**
         * Load from storage, using defaults if required.
         */
        virtual auto LoadSettings() -> std::unique_ptr<ApiSettings> = 0;

        /**
         * Load "new" settings, ie get the user to do whatever they
         * need to do to provide new settings.
         */
        virtual auto NewSettings() -> std::unique_ptr<ApiSettings> = 0;
    };
} // namespace UKControllerPluginUtils::Api
