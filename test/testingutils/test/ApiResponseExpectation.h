#pragma once

namespace UKControllerPluginTest {
    /**
     * An interface for expecting a particular response to happen.
     */
    class ApiResponseExpectation
    {
        public:
        virtual auto WillReturnCreated() -> ApiResponseExpectation& = 0;
        virtual auto WillReturnOk() -> ApiResponseExpectation& = 0;
        virtual auto WillReturnServerError() -> ApiResponseExpectation& = 0;
        virtual auto WillReturnForbidden() -> ApiResponseExpectation& = 0;
        virtual auto WithResponseBody(const nlohmann::json& body) -> ApiResponseExpectation& = 0;
        virtual auto WithInvalidBodyJson() -> ApiResponseExpectation& = 0;
    };
} // namespace UKControllerPluginTest
