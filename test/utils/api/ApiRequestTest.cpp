#include "api/ApiRequest.h"
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"

using UKControllerPluginUtils::Api::ApiRequest;
using UKControllerPluginUtils::Api::ApiRequestData;
using UKControllerPluginUtils::Api::ApiRequestException;
using UKControllerPluginUtils::Api::Response;
using UKControllerPluginUtils::Http::HttpMethod;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtilsTest::Api {
    class ApiRequestTest : public testing::Test
    {
        public:
        ApiRequestTest() : mockResponse(HttpStatusCode::Ok, {}), requestData("someuri", HttpMethod::Get(), {})
        {
            ON_CALL(performer, Perform).WillByDefault(testing::Return(mockResponse));
        }

        [[nodiscard]] auto GetRequest() -> class ApiRequest
        {
            return {requestData, performer, [this]() { requestComplete = true; }};
        };

        bool requestComplete = false;
        Response mockResponse;
        testing::NiceMock<MockApiRequestPerformer> performer;
        ApiRequestData requestData;
    };

    TEST_F(ApiRequestTest, ItExecutesAChainedRequest)
    {
        auto callback1CallTime = (std::chrono::system_clock::time_point::min)();
        auto callback2CallTime = (std::chrono::system_clock::time_point::min)();
        auto callback3CallTime = (std::chrono::system_clock::time_point::min)();
        GetRequest()
            .Then([&callback1CallTime]() -> void {
                callback1CallTime = std::chrono::system_clock::now();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            })
            .Then([&callback2CallTime](Response response) -> void {
                EXPECT_EQ(HttpStatusCode::Ok, response.StatusCode());
                callback2CallTime = std::chrono::system_clock::now();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            })
            .Then([&callback3CallTime](Response response) -> Response {
                callback3CallTime = std::chrono::system_clock::now();
                return response;
            });

        auto testStart = std::chrono::system_clock::now();
        while (true) {
            if (std::chrono::system_clock::now() > testStart + std::chrono::seconds(5)) {
                throw std::exception("Test time exceeded");
            }

            if (requestComplete) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback1CallTime);
        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback2CallTime);
        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback3CallTime);
        EXPECT_TRUE(callback1CallTime < callback2CallTime);
        EXPECT_TRUE(callback2CallTime < callback3CallTime);
    }

    TEST_F(ApiRequestTest, ItExecutesAChainedRequestSynchronously)
    {
        auto callback1CallTime = (std::chrono::system_clock::time_point::min)();
        auto callback2CallTime = (std::chrono::system_clock::time_point::min)();
        auto callback3CallTime = (std::chrono::system_clock::time_point::min)();

        GetRequest()
            .Then([&callback1CallTime]() {
                callback1CallTime = std::chrono::system_clock::now();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            })
            .Then([&callback2CallTime](Response response) {
                EXPECT_EQ(HttpStatusCode::Ok, response.StatusCode());
                callback2CallTime = std::chrono::system_clock::now();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            })
            .Then([&callback3CallTime](Response response) -> Response {
                callback3CallTime = std::chrono::system_clock::now();
                return response;
            })
            .Await();

        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback1CallTime);
        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback2CallTime);
        EXPECT_NE((std::chrono::system_clock::time_point::min)(), callback3CallTime);
        EXPECT_TRUE(callback1CallTime < callback2CallTime);
        EXPECT_TRUE(callback2CallTime < callback3CallTime);
    }

    TEST_F(ApiRequestTest, ItCallsExceptionHandler)
    {
        ON_CALL(performer, Perform)
            .WillByDefault(testing::Throw(ApiRequestException("someuri", HttpStatusCode::ServerError, false)));

        bool exceptionHandlerCalled = false;
        bool successCalled = false;

        try {
            GetRequest()
                .Then([&successCalled]() { successCalled = true; })
                .Catch([&exceptionHandlerCalled](const ApiRequestException&) { exceptionHandlerCalled = true; })
                .Await();
        } catch (...) {
        }

        EXPECT_TRUE(exceptionHandlerCalled);
        EXPECT_FALSE(successCalled);
    }

    TEST_F(ApiRequestTest, ItCallsDefaultExceptionHandler)
    {
        ON_CALL(performer, Perform)
            .WillByDefault(testing::Throw(ApiRequestException("someuri", HttpStatusCode::ServerError, false)));

        bool successCalled = false;

        GetRequest().Then([&successCalled]() { successCalled = true; }).Await();
        EXPECT_FALSE(successCalled);
    }

    TEST_F(ApiRequestTest, ItCallsExceptionHandlerAsynchronously)
    {
        ON_CALL(performer, Perform)
            .WillByDefault(testing::Throw(ApiRequestException("someuri", HttpStatusCode::ServerError, false)));

        bool exceptionHandlerCalled = false;
        bool successCalled = false;

        GetRequest()
            .Then([&successCalled]() { successCalled = true; })
            .Catch([&exceptionHandlerCalled](const ApiRequestException&) { exceptionHandlerCalled = true; });

        auto testStart = std::chrono::system_clock::now();
        while (true) {
            if (std::chrono::system_clock::now() > testStart + std::chrono::seconds(5)) {
                throw std::exception("Test time exceeded");
            }

            if (exceptionHandlerCalled) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        EXPECT_TRUE(exceptionHandlerCalled);
        EXPECT_FALSE(successCalled);
    }
} // namespace UKControllerPluginUtilsTest::Api
