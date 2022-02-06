#pragma once
#include "http/HttpStatusCode.h"

namespace UKControllerPluginUtils::Api {
    /**
     * An exception thrown by the "new" API flow if something goes wrong.
     */
    class ApiRequestException : public std::runtime_error
    {
        public:
        explicit ApiRequestException(const std::string uri, Http::HttpStatusCode statusCode, bool invalidJson);
        [[nodiscard]] auto StatusCode() const -> const Http::HttpStatusCode&;
        [[nodiscard]] auto Uri() const -> const std::string&;
        [[nodiscard]] auto InvalidJson() const -> bool;

        private:
        // The URI this request was trying to hit
        const std::string uri;
        
        // The HTTP status code
        Http::HttpStatusCode statusCode;
        
        // Is the response invalid JSON
        bool invalidJson;
    };
} // namespace UKControllerPluginUtils::Api
