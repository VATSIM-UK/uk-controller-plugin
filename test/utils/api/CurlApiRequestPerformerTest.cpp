#include "api/ApiCurlRequestFactory.h"
#include "api/ApiHeaderApplicator.h"
#include "api/ApiRequestData.h"
#include "api/ApiSettings.h"
#include "api/ApiUrlBuilder.h"
#include "api/ApiRequestException.h"
#include "api/CurlApiRequestPerformer.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginUtils::Api::ApiCurlRequestFactory;
using UKControllerPluginUtils::Api::ApiHeaderApplicator;
using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::ApiUrlBuilder;
using UKControllerPluginUtils::Api::CurlApiRequestPerformer;
using UKControllerPluginUtils::Http::HttpMethod;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtilsTest::Api {
    class CurlApiRequestPerformerTest : public testing::Test
    {
        public:
        CurlApiRequestPerformerTest()
            : requestData("test", HttpMethod::Get()), settings("https://ukcp.vatsim.uk", "key"), builder(settings),
              headerApplicator(settings), requestFactory(builder, headerApplicator), performer(curl, requestFactory)
        {
        }

        [[nodiscard]] static auto GetRequest() -> CurlRequest
        {
            CurlRequest expectedRequest("https://ukcp.vatsim.uk/api/test", CurlRequest::METHOD_GET);
            expectedRequest.AddHeader("Authorization", "Bearer key");
            expectedRequest.AddHeader("Accept", "application/json");
            expectedRequest.AddHeader("Content-Type", "application/json");
            return expectedRequest;
        }

        [[nodiscard]] static auto GetResponseJson() -> std::string
        {
            return nlohmann::json{{"foo", "bar"}}.dump();
        }

        void TestSuccessStatus(HttpStatusCode statusCode)
        {
            EXPECT_CALL(curl, MakeCurlRequest(GetRequest()))
                .Times(1)
                .WillOnce(testing::Return(CurlResponse(GetResponseJson(), false, static_cast<uint64_t>(statusCode))));

            auto response = performer.Perform(requestData);
            EXPECT_EQ(statusCode, response.StatusCode());
            EXPECT_EQ(nlohmann::json({{"foo", "bar"}}), response.Data());
        }

        void TestFailureStatus(HttpStatusCode statusCode)
        {
            EXPECT_CALL(curl, MakeCurlRequest(GetRequest()))
                .Times(1)
                .WillOnce(testing::Return(CurlResponse(GetResponseJson(), false, static_cast<uint64_t>(statusCode))));

            try {
                performer.Perform(requestData);
            } catch (ApiRequestException& exception) {
                EXPECT_EQ("test", exception.Uri());
                EXPECT_EQ(statusCode, exception.StatusCode());
                EXPECT_FALSE(exception.InvalidJson());
                return;
            }

            GTEST_FAIL();
        }

        testing::NiceMock<MockCurlApi> curl;
        ApiRequestData requestData;
        ApiSettings settings;
        ApiUrlBuilder builder;
        ApiHeaderApplicator headerApplicator;
        ApiCurlRequestFactory requestFactory;
        CurlApiRequestPerformer performer;
    };

    TEST_F(CurlApiRequestPerformerTest, ItSucceedsOnOkStatus)
    {
        TestSuccessStatus(HttpStatusCode::Ok);
    }

    TEST_F(CurlApiRequestPerformerTest, ItSucceedsOnCreatedStatus)
    {
        TestSuccessStatus(HttpStatusCode::Created);
    }

    TEST_F(CurlApiRequestPerformerTest, ItSucceedsOnNoContentStatus)
    {
        TestSuccessStatus(HttpStatusCode::NoContent);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnBadRequestStatus)
    {
        TestFailureStatus(HttpStatusCode::BadRequest);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnUnauthorisedStatus)
    {
        TestFailureStatus(HttpStatusCode::Unauthorised);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnForbiddenStatus)
    {
        TestFailureStatus(HttpStatusCode::Forbidden);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnNotFoundStatus)
    {
        TestFailureStatus(HttpStatusCode::NotFound);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnMethodNotAllowedStatus)
    {
        TestFailureStatus(HttpStatusCode::MethodNotAllowed);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnUnprocessableStatus)
    {
        TestFailureStatus(HttpStatusCode::Unprocessable);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnServerErrorStatus)
    {
        TestFailureStatus(HttpStatusCode::ServerError);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnBadGatewayStatus)
    {
        TestFailureStatus(HttpStatusCode::BadGateway);
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnUnknownStatus)
    {
        TestFailureStatus(static_cast<HttpStatusCode>(999L));
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnCurlError)
    {
        EXPECT_CALL(curl, MakeCurlRequest(GetRequest()))
            .Times(1)
            .WillOnce(testing::Return(CurlResponse(GetResponseJson(), true, 500L)));

        try {
            performer.Perform(requestData);
        } catch (ApiRequestException& exception) {
            EXPECT_EQ("test", exception.Uri());
            EXPECT_EQ(HttpStatusCode::ServerError, exception.StatusCode());
            EXPECT_FALSE(exception.InvalidJson());
            return;
        }

        GTEST_FAIL();
    }

    TEST_F(CurlApiRequestPerformerTest, ItFailsOnBadJson)
    {
        EXPECT_CALL(curl, MakeCurlRequest(GetRequest()))
            .Times(1)
            .WillOnce(testing::Return(CurlResponse("{]", false, 200L)));

        try {
            performer.Perform(requestData);
        } catch (ApiRequestException& exception) {
            EXPECT_EQ("test", exception.Uri());
            EXPECT_EQ(HttpStatusCode::Ok, exception.StatusCode());
            EXPECT_TRUE(exception.InvalidJson());
            return;
        }

        GTEST_FAIL();
    }
} // namespace UKControllerPluginUtilsTest::Api
