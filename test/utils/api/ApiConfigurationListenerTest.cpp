#include "api/ApiConfigurationListener.h"
#include "curl/CurlApi.h"
#include "curl/CurlRequest.h"

namespace UKControllerPluginUtilsTest::Api {
    class ApiConfigurationListenerTest : public testing::Test
    {
        public:
        UKControllerPluginUtils::Api::ApiConfigurationListener listener;
        UKControllerPlugin::Curl::CurlApi curl;
    };

    TEST_F(ApiConfigurationListenerTest, ItReturnsBadRequestOnNoKey)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?notkey=abc",
            UKControllerPlugin::Curl::CurlRequest::METHOD_GET));

        EXPECT_EQ(400L, response.GetStatusCode());
        EXPECT_EQ("Invalid request.", response.GetResponse());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsBadRequestOnKeyEmpty)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_GET));

        EXPECT_EQ(400L, response.GetStatusCode());
        EXPECT_EQ("Invalid request.", response.GetResponse());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsNotFoundOnPost)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=abc&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_POST));

        EXPECT_EQ(404L, response.GetStatusCode());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsNotFoundOnPut)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=abc&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_PUT));

        EXPECT_EQ(404L, response.GetStatusCode());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsNotFoundOnDelete)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=abc&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_DELETE));

        EXPECT_EQ(404L, response.GetStatusCode());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsNotFoundOnPatch)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=abc&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_PATCH));

        EXPECT_EQ(404L, response.GetStatusCode());
    }

    TEST_F(ApiConfigurationListenerTest, ItReturnsOkOnGoodKey)
    {
        const auto response = curl.MakeCurlRequest(UKControllerPlugin::Curl::CurlRequest(
            "http://" + listener.Host() + ":" + std::to_string(listener.Port()) + "?key=abc&foo=bar",
            UKControllerPlugin::Curl::CurlRequest::METHOD_GET));

        EXPECT_EQ(200L, response.GetStatusCode());
        EXPECT_EQ(
            "UK Controller Plugin API key received successfully. You may now close this window.",
            response.GetResponse());
    }
} // namespace UKControllerPluginUtilsTest::Api
