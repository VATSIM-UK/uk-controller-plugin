#include "api/ApiRequestData.h"
#include "api/ApiRequestFactory.h"

using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestFactory;
using UKControllerPluginUtils::Http::HttpMethod;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtilsTest::Api {
    class ApiRequestFactoryTest : public testing::Test
    {
        public:
        ApiRequestFactoryTest() : response(HttpStatusCode::Ok, {}), factory(requestPerformer)
        {
        }

        Response response;
        testing::NiceMock<MockApiRequestPerformer> requestPerformer;
        ApiRequestFactory factory;
    };

    TEST_F(ApiRequestFactoryTest, ItPerformsAGetRequest)
    {
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("someuri", HttpMethod::Get())))
            .Times(1)
            .WillOnce(testing::Return(response));

        bool callbackPerformed = false;

        factory.Get("someuri").Then([&callbackPerformed]() { callbackPerformed = true; });

        factory.AwaitRequestCompletion();
    }

    TEST_F(ApiRequestFactoryTest, ItPerformsAPostRequest)
    {
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("someuri", HttpMethod::Post(), {{"foo", "bar"}})))
            .Times(1)
            .WillOnce(testing::Return(response));

        bool callbackPerformed = false;

        factory.Post("someuri", {{"foo", "bar"}}).Then([&callbackPerformed]() { callbackPerformed = true; });

        factory.AwaitRequestCompletion();
    }

    TEST_F(ApiRequestFactoryTest, ItPerformsAPutRequest)
    {
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("someuri", HttpMethod::Put(), {{"foo", "bar"}})))
            .Times(1)
            .WillOnce(testing::Return(response));

        bool callbackPerformed = false;

        factory.Put("someuri", {{"foo", "bar"}}).Then([&callbackPerformed]() { callbackPerformed = true; });

        factory.AwaitRequestCompletion();
    }

    TEST_F(ApiRequestFactoryTest, ItPerformsAPatchRequest)
    {
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("someuri", HttpMethod::Patch(), {{"foo", "bar"}})))
            .Times(1)
            .WillOnce(testing::Return(response));

        bool callbackPerformed = false;

        factory.Patch("someuri", {{"foo", "bar"}}).Then([&callbackPerformed]() { callbackPerformed = true; });

        factory.AwaitRequestCompletion();
    }

    TEST_F(ApiRequestFactoryTest, ItPerformsADeleteRequest)
    {
        EXPECT_CALL(requestPerformer, Perform(ApiRequestData("someuri", HttpMethod::Delete())))
            .Times(1)
            .WillOnce(testing::Return(response));

        bool callbackPerformed = false;

        factory.Delete("someuri").Then([&callbackPerformed]() { callbackPerformed = true; });

        factory.AwaitRequestCompletion();
    }
} // namespace UKControllerPluginUtilsTest::Api
