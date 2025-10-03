#pragma once
#include <memory>
#include <string>
#include <variant>
#include "ApiMethodExpectation.h"
#include "ApiRequestExpectation.h"
#include "ApiResponseExpectation.h"
#include "ApiUriExpectation.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/Response.h"
#include "http/HttpStatusCode.h"

namespace UKControllerPluginUtils::Http {
    class HttpMethod;
} // namespace UKControllerPluginUtils::Http

namespace UKControllerPluginUtilsTest::Api {
    class MockApiRequestPerformer;
} // namespace UKControllerPluginUtilsTest::Api

namespace UKControllerPluginTest {
    /**
     * Builds up expectations during API tests in a more fluent way.
     */
    class ApiExpectation : public ApiMethodExpectation,
                           public ApiRequestExpectation,
                           public ApiResponseExpectation,
                           public ApiUriExpectation
    {
        public:
        ApiExpectation(bool isPositive, UKControllerPluginUtilsTest::Api::MockApiRequestPerformer& performer);
        ~ApiExpectation();
        auto Get() -> ApiUriExpectation& override;
        auto Post() -> ApiUriExpectation& override;
        auto Put() -> ApiUriExpectation& override;
        auto Patch() -> ApiUriExpectation& override;
        auto Delete() -> ApiUriExpectation& override;
        auto WithBody(const nlohmann::json& body) -> ApiResponseExpectation& override;
        auto WithoutBody() -> ApiResponseExpectation& override;
        auto WillReturnCreated() -> ApiResponseExpectation& override;
        auto WillReturnOk() -> ApiResponseExpectation& override;
        auto WillReturnServerError() -> ApiResponseExpectation& override;
        auto WillReturnForbidden() -> ApiResponseExpectation& override;
        auto WithResponseBody(const nlohmann::json& body) -> ApiResponseExpectation& override;
        auto WithInvalidBodyJson() -> ApiResponseExpectation& override;
        auto To(const std::string& uri) -> ApiRequestExpectation& override;

        private:
        [[nodiscard]] auto ExpectedRequest() const -> UKControllerPluginUtils::Api::ApiRequestData;
        [[nodiscard]] auto ExpectedResponse() const
            -> std::variant<UKControllerPluginUtils::Api::Response, UKControllerPluginUtils::Api::ApiRequestException>;

        // Is this a positive or negative assertion
        bool isPositive;

        // Performs the expectation
        UKControllerPluginUtilsTest::Api::MockApiRequestPerformer& performer;

        // The expected URI for the request
        std::string uri;

        // The expected method
        std::shared_ptr<UKControllerPluginUtils::Http::HttpMethod> method;

        // The expected body
        nlohmann::json requestBody;

        // The response HTTP status code
        UKControllerPluginUtils::Http::HttpStatusCode responseCode;

        // The expected response body
        nlohmann::json responseBody;

        // The response body is invalid
        bool invalidBodyJson = false;
    };
} // namespace UKControllerPluginTest
