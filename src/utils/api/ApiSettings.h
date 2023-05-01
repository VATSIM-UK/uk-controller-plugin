#pragma once

namespace UKControllerPluginUtils::Api {
    /**
     * Contains the settings for the API
     */
    class ApiSettings
    {
        public:
        ApiSettings(std::string url, std::string key);
        [[nodiscard]] auto Url() const -> const std::string&;
        void Url(const std::string& url);
        [[nodiscard]] auto Key() const -> const std::string&;
        void Key(const std::string& key);

        private:
        // The URL for the api
        std::string url;

        // The users API key
        std::string key;

        // Mutex for async access
        mutable std::mutex mutex;
    };
} // namespace UKControllerPluginUtils::Api
