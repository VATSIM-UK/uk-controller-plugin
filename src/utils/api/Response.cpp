#include "Response.h"

namespace UKControllerPluginUtils::Api {

    Response::Response(Http::HttpStatusCode statusCode, nlohmann::json data)
        : statusCode(statusCode), data(std::move(data))
    {
    }

    auto Response::StatusCode() const -> Http::HttpStatusCode
    {
        return statusCode;
    }

    auto Response::Data() const -> const nlohmann::json&
    {
        return data;
    }
} // namespace UKControllerPluginUtils::Api
