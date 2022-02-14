#pragma once
#include "api/ApiRequestData.h"
#include "api/ApiRequestException.h"
#include "api/Response.h"

namespace UKControllerPluginUtils::Api {
    class ApiFactory;
    class ApiRequestData;
    class ApiRequestException;
    class ApiRequestFactory;
    class ApiSettings;
    class Response;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPluginUtilsTest::Api {
    class MockApiRequestPerformer;
    class MockApiRequestPerformerFactory;
    class MockApiSettingsProvider;
} // namespace UKControllerPluginUtilsTest::Api

namespace UKControllerPluginTest {
    class ApiTestCase : public testing::Test
    {
        public:
        ApiTestCase();
        ~ApiTestCase() = default;
        void ExpectApiRequestWithResponse(
            const UKControllerPluginUtils::Api::ApiRequestData& request,
            const UKControllerPluginUtils::Api::Response& response);
        void ExpectApiRequestWithError(
            const UKControllerPluginUtils::Api::ApiRequestData& request,
            const UKControllerPluginUtils::Api::ApiRequestException& exception);
        void ExpectNoRequests();
        void DontExpectRequest(const UKControllerPluginUtils::Api::ApiRequestData& request);
        void AwaitApiCallCompletion();
        [[nodiscard]] auto SettingsProvider()
            -> testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider>&;

        private:
        std::shared_ptr<UKControllerPluginUtils::Api::ApiSettings> settings;

        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiRequestPerformer>> requestPerformer;
        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiRequestPerformerFactory>>
            requestPerformerFactory;
        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider>> settingsProvider;

        std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> factory;
    };
} // namespace UKControllerPluginTest
