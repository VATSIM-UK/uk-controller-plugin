#include "ApiSettings.h"

namespace UKControllerPluginUtils::Api {

    ApiSettings::ApiSettings(std::string url, std::string key) : url(std::move(url)), key(std::move(key))
    {
    }

    auto ApiSettings::Url() const -> const std::string&
    {
        const auto lock = std::unique_lock(mutex);
        return url;
    }

    void ApiSettings::Url(const std::string& url)
    {
        const auto lock = std::unique_lock(mutex);
        this->url = url;
    }

    auto ApiSettings::Key() const -> const std::string&
    {
        const auto lock = std::unique_lock(mutex);
        return key;
    }

    void ApiSettings::Key(const std::string& key)
    {
        const auto lock = std::unique_lock(mutex);
        this->key = key;
    }
} // namespace UKControllerPluginUtils::Api
