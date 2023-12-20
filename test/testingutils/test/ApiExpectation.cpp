#include "ApiExpectation.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/Response.h"
#include "http/HttpMethod.h"

using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::Response;
using UKControllerPluginUtils::Http::HttpMethod;
using UKControllerPluginUtils::Http::HttpStatusCode;
using UKControllerPluginUtils::Http::IsSuccessful;

namespace UKControllerPluginTest {

    ApiExpectation::ApiExpectation(
        bool isPositive, UKControllerPluginUtilsTest::Api::MockApiRequestPerformer& performer)
        : isPositive(isPositive), performer(performer), responseCode(HttpStatusCode::Unknown)
    {
    }

    ApiExpectation::~ApiExpectation()
    {
        if (this->isPositive) {
            if (std::holds_alternative<Response>(ExpectedResponse())) {
                EXPECT_CALL(performer, Perform(ExpectedRequest()))
                    .WillOnce(testing::Return(std::get<Response>(ExpectedResponse())))
                    .RetiresOnSaturation();
            } else {
                EXPECT_CALL(performer, Perform(ExpectedRequest()))
                    .WillOnce(testing::Throw(std::get<ApiRequestException>(ExpectedResponse())))
                    .RetiresOnSaturation();
            }
        } else {
            EXPECT_CALL(performer, Perform(ExpectedRequest())).Times(0);
        }
    }

    ApiUriExpectation& ApiExpectation::Get()
    {
        this->method = std::make_shared<HttpMethod>(HttpMethod::Get());
        return *this;
    }

    ApiUriExpectation& ApiExpectation::Post()
    {
        this->method = std::make_shared<HttpMethod>(HttpMethod::Post());
        return *this;
    }

    ApiUriExpectation& ApiExpectation::Put()
    {
        this->method = std::make_shared<HttpMethod>(HttpMethod::Put());
        return *this;
    }

    ApiUriExpectation& ApiExpectation::Patch()
    {
        this->method = std::make_shared<HttpMethod>(HttpMethod::Patch());
        return *this;
    }

    ApiUriExpectation& ApiExpectation::Delete()
    {
        this->method = std::make_shared<HttpMethod>(HttpMethod::Delete());
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WithBody(const nlohmann::json& body)
    {
        this->requestBody = body;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WithoutBody()
    {
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WillReturnCreated()
    {
        this->responseCode = HttpStatusCode::Created;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WillReturnOk()
    {
        this->responseCode = HttpStatusCode::Ok;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WillReturnServerError()
    {
        this->responseCode = HttpStatusCode::ServerError;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WillReturnForbidden()
    {
        this->responseCode = HttpStatusCode::Forbidden;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WithResponseBody(const nlohmann::json& body)
    {
        this->responseBody = body;
        return *this;
    }

    ApiResponseExpectation& ApiExpectation::WithInvalidBodyJson()
    {
        invalidBodyJson = true;
        return *this;
    }

    ApiRequestExpectation& ApiExpectation::To(const std::string& uri)
    {
        this->uri = uri;
        return *this;
    }

    auto ApiExpectation::ExpectedResponse() const
        -> std::variant<UKControllerPluginUtils::Api::Response, UKControllerPluginUtils::Api::ApiRequestException>
    {
        if (IsSuccessful(this->responseCode) && !invalidBodyJson) {
            return {Response(this->responseCode, this->responseBody)};
        }

        return {ApiRequestException(uri, responseCode, invalidBodyJson)};
    }

    auto ApiExpectation::ExpectedRequest() const -> UKControllerPluginUtils::Api::ApiRequestData
    {
        return {uri, *method, requestBody};
    }
} // namespace UKControllerPluginTest
