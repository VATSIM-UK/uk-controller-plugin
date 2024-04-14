#pragma once
#include "ApiMethodExpectation.h"
#include "ApiUriExpectation.h"
#include "ApiRequestExpectation.h"
#include "ApiResponseExpectation.h"

namespace UKControllerPluginUtils::Api {
    class ApiFactory;
    class ApiRequestFactory;
    class ApiSettings;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPluginUtilsTest::Api {
    class MockApiRequestPerformer;
    class MockApiRequestPerformerFactory;
    class MockApiSettingsProvider;
} // namespace UKControllerPluginUtilsTest::Api

namespace UKControllerPluginTest {
    class ApiTestCase : public virtual testing::Test
    {
        public:
        ApiTestCase();
        ~ApiTestCase() = default;
        void TearDown() override;
        [[nodiscard]] auto ExpectApiRequest() -> std::shared_ptr<ApiMethodExpectation>;
        [[nodiscard]] auto DontExpectApiRequest() -> std::shared_ptr<ApiMethodExpectation>;
        void ExpectNoApiRequests();
        void AwaitApiCallCompletion();
        [[nodiscard]] auto
        SettingsProvider() -> testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider>&;

        private:
        std::shared_ptr<UKControllerPluginUtils::Api::ApiSettings> settings;

        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiRequestPerformer>> requestPerformer;
        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiRequestPerformerFactory>>
            requestPerformerFactory;
        std::shared_ptr<testing::NiceMock<UKControllerPluginUtilsTest::Api::MockApiSettingsProvider>> settingsProvider;

        std::shared_ptr<UKControllerPluginUtils::Api::ApiFactory> factory;
    };
} // namespace UKControllerPluginTest
