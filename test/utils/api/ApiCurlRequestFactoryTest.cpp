#include "api/ApiCurlRequestFactory.h"
#include "api/ApiUrlBuilder.h"
#include "api/ApiHeaderApplicator.h"
#include "api/ApiRequestData.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginUtils::Api::ApiCurlRequestFactory;
using UKControllerPluginUtils::Api::ApiHeaderApplicator;
using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::ApiUrlBuilder;

namespace UKControllerPluginUtilsTest::Api {
    class ApiCurlRequestFactoryTest : public testing::Test
    {
        public:
        ApiCurlRequestFactoryTest()
            : settings("https://ukcp.vatsim.uk", "key"), builder(settings), headerApplicator(settings),
              requestFactory(builder, headerApplicator)
        {
        }
        ApiSettings settings;
        ApiUrlBuilder builder;
        ApiHeaderApplicator headerApplicator;
        ApiCurlRequestFactory requestFactory;
    };

    TEST_F(ApiCurlRequestFactoryTest, ItBuildsRequests)
    {
        auto request =
            requestFactory.BuildCurlRequest(ApiRequestData("test", UKControllerPluginUtils::Http::HttpMethod::Get()));

        CurlRequest expectedRequest("https://ukcp.vatsim.uk/api/test", CurlRequest::METHOD_GET);
        expectedRequest.AddHeader("Authorization", "Bearer key");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");

        EXPECT_EQ(expectedRequest, request);
    }

    TEST_F(ApiCurlRequestFactoryTest, ItBuildsRequestsWithABody)
    {
        const auto body = nlohmann::json{{"test", "test"}};
        auto request = requestFactory.BuildCurlRequest(
            ApiRequestData("test", UKControllerPluginUtils::Http::HttpMethod::Post(), body));

        CurlRequest expectedRequest("https://ukcp.vatsim.uk/api/test", CurlRequest::METHOD_POST);
        expectedRequest.AddHeader("Authorization", "Bearer key");
        expectedRequest.AddHeader("Accept", "application/json");
        expectedRequest.AddHeader("Content-Type", "application/json");
        expectedRequest.SetBody(body.dump());

        EXPECT_EQ(expectedRequest, request);
    }
} // namespace UKControllerPluginUtilsTest::Api
