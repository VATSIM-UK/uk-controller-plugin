#pragma once
#include "http/HttpStatusCode.h"

namespace UKControllerPluginUtils::Api {
    /**
     * Represents a response from the API
     */
    class Response
    {
        public:
        Response(Http::HttpStatusCode statusCode, nlohmann::json data);
        [[nodiscard]] auto StatusCode() const -> Http::HttpStatusCode;
        [[nodiscard]] auto Data() const -> const nlohmann::json&;

        private:
        // The status code
        Http::HttpStatusCode statusCode;

        // The data that came back
        nlohmann::json data;
    };
} // namespace UKControllerPluginUtils::Api
