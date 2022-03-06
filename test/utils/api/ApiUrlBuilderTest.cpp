#include "api/ApiRequestData.h"
#include "api/ApiUrlBuilder.h"
#include "api/ApiSettings.h"

using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::ApiUrlBuilder;
using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPluginUtilsTest::Api {
    class ApiUrlBuilderTest : public testing::Test
    {
    };

    TEST_F(ApiUrlBuilderTest, ItBuildsUrl)
    {
        auto settings = ApiSettings("https://ukcp.vatsim.uk", "key");
        auto requestData = ApiRequestData("test", HttpMethod::Get());
        auto builder = ApiUrlBuilder(settings);

        EXPECT_EQ("https://ukcp.vatsim.uk/test", builder.BuildUrl(requestData));
    }

    TEST_F(ApiUrlBuilderTest, ItBuildsUrlRightTrimmingTheBaseUrl)
    {
        auto settings = ApiSettings("https://ukcp.vatsim.uk/", "key");
        auto requestData = ApiRequestData("test", HttpMethod::Get());
        auto builder = ApiUrlBuilder(settings);

        EXPECT_EQ("https://ukcp.vatsim.uk/test", builder.BuildUrl(requestData));
    }

    TEST_F(ApiUrlBuilderTest, ItBuildsUrlLeftTrimmingTheUri)
    {
        auto settings = ApiSettings("https://ukcp.vatsim.uk", "key");
        auto requestData = ApiRequestData("/test", HttpMethod::Get());
        auto builder = ApiUrlBuilder(settings);

        EXPECT_EQ("https://ukcp.vatsim.uk/test", builder.BuildUrl(requestData));
    }

    TEST_F(ApiUrlBuilderTest, ItBuildsUrlRightTrimmingTheUri)
    {
        auto settings = ApiSettings("https://ukcp.vatsim.uk", "key");
        auto requestData = ApiRequestData("test/", HttpMethod::Get());
        auto builder = ApiUrlBuilder(settings);

        EXPECT_EQ("https://ukcp.vatsim.uk/test", builder.BuildUrl(requestData));
    }
} // namespace UKControllerPluginUtilsTest::Api
