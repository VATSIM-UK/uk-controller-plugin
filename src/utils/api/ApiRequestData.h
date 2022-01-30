#pragma once
#include "http/HttpMethod.h"

namespace UKControllerPluginUtils::Api {
    /**
     * Contains the data for an API request
     */
    class ApiRequestData
    {
        public:
        ApiRequestData(std::string uri, Http::HttpMethod method, nlohmann::json body = {});
        [[nodiscard]] auto Uri() const -> const std::string&;
        [[nodiscard]] auto Method() const -> Http::HttpMethod;
        [[nodiscard]] auto Body() const -> const nlohmann::json&;

        private:
        void CheckForRequiredBody();
        [[nodiscard]] auto MethodRequiresBody() -> bool;
        [[nodiscard]] auto BodyMissing() -> bool;

        // The URI to hit
        std::string uri;

        // The HTTP method being called
        Http::HttpMethod method;

        // The body of the request, for PUT and POST
        nlohmann::json body;
    };
} // namespace UKControllerPluginUtils::Api
