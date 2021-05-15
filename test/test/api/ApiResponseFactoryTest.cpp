#include "pch/utilstestpch.h"
#include "api/ApiResponseFactory.h"
#include "api/ApiResponse.h"
#include "curl/CurlResponse.h"

using UKControllerPlugin::Api::ApiResponseFactory;
using UKControllerPlugin::Api::ApiResponse;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginUtilsTest {
    namespace Api {

        TEST(ApiResponseFactory, CreateReturnsCorrectlyIfResponseNotJson)
        {
            CurlResponse curl("blalalalal", false, 200);
            ApiResponse api = ApiResponseFactory::Create(curl);

            EXPECT_TRUE(api.IsSuccess());
            EXPECT_TRUE("No further details were provided." == api.GetResponseMessage());
            EXPECT_FALSE(api.IsCurlError());
        }

        TEST(ApiResponseFactory, CreateReturnsCorrectlyIfCurlError)
        {
            CurlResponse curl("blalalalal", true, -1);
            ApiResponse api = ApiResponseFactory::Create(curl);

            EXPECT_FALSE(api.IsSuccess());
            EXPECT_TRUE("No further details were provided." == api.GetResponseMessage());
            EXPECT_TRUE(api.IsCurlError());
        }

        TEST(ApiResponseFactory, CreateReturnsCorrectlyIfNotSuccessfulWithMessageNotString)
        {
            CurlResponse curl("{\"message\": 123}", false, 400);
            ApiResponse api = ApiResponseFactory::Create(curl);

            EXPECT_TRUE("No further details were provided." == api.GetResponseMessage());
        }

        TEST(ApiResponseFactory, CreateReturnsCorrectlyWithNoMessage)
        {
            CurlResponse curl("{\"bla\": false}", false, 400);
            ApiResponse api = ApiResponseFactory::Create(curl);

            EXPECT_TRUE("No further details were provided." == api.GetResponseMessage());
        }

        TEST(ApiResponseFactory, CreateDoesNotAlterJson)
        {
            CurlResponse curl("{\"bla\": true}", false, 200);
            EXPECT_EQ("{\"bla\": true}"_json, ApiResponseFactory::Create(curl).GetRawData());
        }

        TEST(ApiResponseFactory, SetsTheStatusCodeCorrectly)
        {
            CurlResponse curl("{\"bla\": true}", false, 200);
            EXPECT_EQ(200L, ApiResponseFactory::Create(curl).GetStatusCode());
        }
    }  // namespace Api
}  // namespace UKControllerPluginUtilsTest
