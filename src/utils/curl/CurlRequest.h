#pragma once
#include "http/HttpMethod.h"

namespace UKControllerPlugin::Curl {

    /*
        Class representing a curl request to the interwebs.
    */
    class CurlRequest
    {

        public:
        CurlRequest(std::string uri, std::string method);
        CurlRequest(std::string uri, UKControllerPluginUtils::Http::HttpMethod method);
        void AddHeader(const std::string& key, std::string value);
        [[nodiscard]] auto GetBody() const -> const char*;
        [[nodiscard]] auto GetMethod() const -> const char*;
        [[nodiscard]] auto GetUri() const -> const char*;
        auto operator==(const CurlRequest& compare) const -> bool;
        [[nodiscard]] auto GetMaxRequestTime() const -> INT64;
        void SetMaxRequestTime(INT64 requestTime);
        void SetBody(const std::string& body);

        // No request body
        inline static const std::string nobody;

        // HTTP Verbs
        inline static const std::string METHOD_GET = "GET";       // NOLINT
        inline static const std::string METHOD_POST = "POST";     // NOLINT
        inline static const std::string METHOD_PUT = "PUT";       // NOLINT
        inline static const std::string METHOD_DELETE = "DELETE"; // NOLINT
        inline static const std::string METHOD_PATCH = "PATCH";   // NOLINT

        using HttpHeaders = std::map<std::string, std::string>;
        using const_iterator = HttpHeaders::const_iterator;
        [[nodiscard]] auto cbegin() const -> const_iterator
        {
            return this->headers.cbegin();
        }
        [[nodiscard]] auto cend() const -> const_iterator
        {
            return this->headers.cend();
        }

        private:
        static auto CheckMethod(std::string method) -> std::string;

        // One of the HTTP verbs
        std::string method;

        // The URL to hit
        std::string uri;

        // The body of the request
        std::string body;

        // Headers
        std::map<std::string, std::string> headers;

        // The maximum amount of time that requests are allowed to take
        INT64 maxRequestTime = 10L; // NOLINT
    };
} // namespace UKControllerPlugin::Curl
