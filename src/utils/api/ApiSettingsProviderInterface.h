#pragma once
#include "ApiBootstrap.h"

namespace UKControllerPluginUtils::Api {
    class ApiSettings;

    class ApiSettingsProviderInterface
    {
        public:
        virtual ~ApiSettingsProviderInterface() = default;

        /**
         * Load from storage, using defaults if required.
         */
        virtual auto Get() -> ApiSettings& = 0;
        
        /**
         * Do we have the API settings?
         */
        virtual auto Has() -> bool = 0;

        /**
         * Triggers a reload of the settings from source.
         */
        [[nodiscard]] virtual auto Reload() -> bool = 0;
    };
} // namespace UKControllerPluginUtils::Api
