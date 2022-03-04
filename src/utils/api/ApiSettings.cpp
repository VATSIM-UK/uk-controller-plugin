#include "ApiSettings.h"

namespace UKControllerPluginUtils::Api {

    ApiSettings::ApiSettings(std::string url, std::string key) : url(std::move(url)), key(std::move(key))
    {
    }

    auto ApiSettings::Url() const -> const std::string&
    {
        return url;
    }

    void ApiSettings::Url(const std::string& url)
    {
        this->url = url;
    }

    auto ApiSettings::Key() const -> const std::string&
    {
        return key;
    }

    void ApiSettings::Key(const std::string& key)
    {
        this->key = key;
    }
} // namespace UKControllerPluginUtils::Api
