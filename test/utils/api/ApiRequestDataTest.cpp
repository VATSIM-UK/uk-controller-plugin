#include "api/ApiRequestData.h"

using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPluginUtilsTest::Api {
    class ApiRequestDataTest : public testing::Test
    {
    };

    TEST_F(ApiRequestDataTest, ItBuildsGetRequest)
    {
        ApiRequestData data("uri", HttpMethod::Get());
        EXPECT_EQ("uri", data.Uri());
        EXPECT_EQ(HttpMethod::Get(), data.Method());
        EXPECT_TRUE(data.Body().empty());
    }

    TEST_F(ApiRequestDataTest, ItBuildsPostRequest)
    {
        nlohmann::json body = {{"foo", "bar"}};
        ApiRequestData data("uri", HttpMethod::Post(), body);
        EXPECT_EQ("uri", data.Uri());
        EXPECT_EQ(HttpMethod::Post(), data.Method());
        EXPECT_EQ(body, data.Body());
    }

    TEST_F(ApiRequestDataTest, ItThrowsExceptionPostRequestWithNoBody)
    {
        EXPECT_THROW(ApiRequestData data("uri", HttpMethod::Post()), std::invalid_argument);
    }

    TEST_F(ApiRequestDataTest, ItBuildsPutRequest)
    {
        nlohmann::json body = {{"foo", "bar"}};
        ApiRequestData data("uri", HttpMethod::Put(), body);
        EXPECT_EQ("uri", data.Uri());
        EXPECT_EQ(HttpMethod::Put(), data.Method());
        EXPECT_EQ(body, data.Body());
    }

    TEST_F(ApiRequestDataTest, ItThrowsExceptionPutRequestWithNoBody)
    {
        EXPECT_THROW(ApiRequestData data("uri", HttpMethod::Put()), std::invalid_argument);
    }

    TEST_F(ApiRequestDataTest, ItBuildsPatchRequest)
    {
        nlohmann::json body = {{"foo", "bar"}};
        ApiRequestData data("uri", HttpMethod::Patch(), body);
        EXPECT_EQ("uri", data.Uri());
        EXPECT_EQ(HttpMethod::Patch(), data.Method());
        EXPECT_EQ(body, data.Body());
    }

    TEST_F(ApiRequestDataTest, ItThrowsExceptionPatchRequestWithNoBody)
    {
        EXPECT_THROW(ApiRequestData data("uri", HttpMethod::Patch()), std::invalid_argument);
    }

    TEST_F(ApiRequestDataTest, ItBuildsDeleteRequest)
    {
        ApiRequestData data("uri", HttpMethod::Delete());
        EXPECT_EQ("uri", data.Uri());
        EXPECT_EQ(HttpMethod::Delete(), data.Method());
        EXPECT_TRUE(data.Body().empty());
    }

    TEST_F(ApiRequestDataTest, EqualityReturnsTrueIfSame)
    {
        ApiRequestData data1("uri", HttpMethod::Patch(), {{"foo", "bar"}});
        ApiRequestData data2("uri", HttpMethod::Patch(), {{"foo", "bar"}});

        EXPECT_TRUE(data1 == data2);
    }

    TEST_F(ApiRequestDataTest, EqualityReturnsFalseIfUriDifferent)
    {
        ApiRequestData data1("uri", HttpMethod::Patch(), {{"foo", "bar"}});
        ApiRequestData data2("uri2", HttpMethod::Patch(), {{"foo", "bar"}});

        EXPECT_FALSE(data1 == data2);
    }

    TEST_F(ApiRequestDataTest, EqualityReturnsFalseIfMethodDifferent)
    {
        ApiRequestData data1("uri", HttpMethod::Patch(), {{"foo", "bar"}});
        ApiRequestData data2("uri", HttpMethod::Post(), {{"foo", "bar"}});

        EXPECT_FALSE(data1 == data2);
    }

    TEST_F(ApiRequestDataTest, EqualityReturnsFalseIfBodyDifferent)
    {
        ApiRequestData data1("uri", HttpMethod::Patch(), {{"foo", "bar"}});
        ApiRequestData data2("uri", HttpMethod::Patch(), {{"baz", "bosh"}});

        EXPECT_FALSE(data1 == data2);
    }
} // namespace UKControllerPluginUtilsTest::Api
