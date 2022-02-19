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
        [[nodiscard]] auto operator==(const ApiRequestData& compare) const -> bool;
        [[nodiscard]] auto operator!=(const ApiRequestData& compare) const -> bool;

        private:
        void CheckForRequiredBody();
        [[nodiscard]] auto MethodRequiresBody() const -> bool;
        [[nodiscard]] auto BodyMissing() const -> bool;
        [[nodiscard]] auto IsEqual(const ApiRequestData& compare) const -> bool;

        // The URI to hit
        std::string uri;

        // The HTTP method being called
        Http::HttpMethod method;

        // The body of the request, for PUT and POST
        nlohmann::json body;
    };
} // namespace UKControllerPluginUtils::Api
