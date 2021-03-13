#include "pch/utilstestpch.h"
#include "api/ApiResponse.h"

using UKControllerPlugin::Api::ApiResponse;

namespace UKControllerPluginUtilsTest {
    namespace Api {
        TEST(ApiResponse, GetResponseMessageReturnsMessage)
        {
            ApiResponse response(true, 200L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.GetResponseMessage() == "Test Message");
        }

        TEST(ApiResponse, GetRawDataReturnsRawData)
        {
            nlohmann::json expected = "{\"test\": 1.20}"_json;
            ApiResponse response(false, 200L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(expected == response.GetRawData());
        }

        TEST(ApiResponse, IsSuccessReturnsTrueOnOk)
        {
            ApiResponse response(false, 200L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsSuccess());
        }

        TEST(ApiResponse, IsSuccessReturnsTrueOnCreated)
        {
            ApiResponse response(false, 201L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsSuccess());
        }

        TEST(ApiResponse, IsSuccessReturnsTrueOnNoContent)
        {
            ApiResponse response(false, 204L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsSuccess());
        }

        TEST(ApiResponse, IsSuccessReturnsFalseOnError)
        {
            ApiResponse response(false, 500L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_FALSE(response.IsSuccess());
        }

        TEST(ApiResponse, IsUnauthorisedReturnsTrueOnUnauthorised)
        {
            ApiResponse response(false, 401L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsUnauthorised());
        }

        TEST(ApiResponse, IsUnuthorisedReturnsTrueOnForbidden)
        {
            ApiResponse response(false, 403L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsUnauthorised());
        }

        TEST(ApiResponse, IsBadRequestReturnsTrueOnBadRequest)
        {
            ApiResponse response(false, 400L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsBadRequest());
        }

        TEST(ApiResponse, IsCurlErrorReturnsTrueOnCurlError)
        {
            ApiResponse response(true, -1L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsCurlError());
        }

        TEST(ApiResponse, IsNotFoundReturnsTrueOnNotFound)
        {
            ApiResponse response(true, 404L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsNotFound());
        }

        TEST(ApiResponse, IsServerErrorReturnsTrueOnInternalServerError)
        {
            ApiResponse response(true, 500L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsServerError());
        }

        TEST(ApiResponse, IsServerErrorReturnsTrueOnServiceUnavailable)
        {
            ApiResponse response(true, 503L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsServerError());
        }

        TEST(ApiResponse, IsTeapotReturnsTrueOnTeapot)
        {
            ApiResponse response(true, 418L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_TRUE(response.IsTeapot());
        }

        TEST(ApiResponse, GetStatusCodeReturnsStatusCode)
        {
            ApiResponse response(true, 418L, "{\"test\": 1.20}"_json, "Test Message");
            EXPECT_EQ(418, response.GetStatusCode());
        }
    }  // namespace Api
}  // namespace UKControllerPluginUtilsTest
