#include "ApiRequestException.h"

namespace UKControllerPluginUtils::Api {

    ApiRequestException::ApiRequestException(const std::string uri, Http::HttpStatusCode statusCode, bool invalidJson)
        : std::runtime_error("Api request resulted in status " + std::to_string(static_cast<uint64_t>(statusCode))),
          uri(uri), statusCode(statusCode), invalidJson(invalidJson)
    {
    }

    auto ApiRequestException::StatusCode() const -> const Http::HttpStatusCode&
    {
        return statusCode;
    }

    auto ApiRequestException::Uri() const -> const std::string&
    {
        return uri;
    }

    auto ApiRequestException::InvalidJson() const -> bool
    {
        return invalidJson;
    }
} // namespace UKControllerPluginUtils::Api
