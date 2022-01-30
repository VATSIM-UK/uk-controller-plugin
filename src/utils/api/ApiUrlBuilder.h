#pragma once
#include "ApiSettings.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestData;
    class ApiSettings;
    
    /**
     * Builds API URLs from a URI
     */
    class ApiUrlBuilder
    {
        public:
        ApiUrlBuilder(const ApiSettings& settings);
        [[nodiscard]] auto BuildUrl(const ApiRequestData& requestData) const -> const std::string;
        
        private:
        const ApiSettings& settings;
    };
} // namespace UKControllerPluginUtils::Api
