#pragma once

namespace UKControllerPlugin::Curl {
    class CurlRequest;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPluginUtils::Api {
    class ApiSettings;

    /**
     * Applies necessary headers to the API requests.
     */
    class ApiHeaderApplicator
    {
        public:
        ApiHeaderApplicator(const ApiSettings& settings);
        void ApplyHeaders(UKControllerPlugin::Curl::CurlRequest& request) const;

        private:
        // Stores API settings including the key
        const ApiSettings& settings;
    };
} // namespace UKControllerPluginUtils::Api
