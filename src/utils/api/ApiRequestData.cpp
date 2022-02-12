#include "ApiRequestData.h"

using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPluginUtils::Api {

    ApiRequestData::ApiRequestData(std::string uri, Http::HttpMethod method, nlohmann::json body)
        : uri(uri), method(method), body(std::move(body))
    {
        this->CheckForRequiredBody();
    }

    auto ApiRequestData::BodyMissing() -> bool
    {
        return body.empty();
    }

    auto ApiRequestData::MethodRequiresBody() -> bool
    {
        return method == HttpMethod::Post() || method == HttpMethod::Put();
    }

    void ApiRequestData::CheckForRequiredBody()
    {
        if (MethodRequiresBody() && BodyMissing()) {
            throw std::invalid_argument("PUT and POST requests require a body");
        }
    }

    auto ApiRequestData::Uri() const -> const std::string&
    {
        return uri;
    }

    auto ApiRequestData::Method() const -> HttpMethod
    {
        return method;
    }

    auto ApiRequestData::Body() const -> const nlohmann::json&
    {
        return body;
    }
} // namespace UKControllerPluginUtils::Api