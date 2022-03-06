#pragma once

namespace UKControllerPluginTest {
    class ApiResponseExpectation;

    /**
     * An interface for expecting a particular request to happen.
     */
    class ApiRequestExpectation
    {
        public:
        [[nodiscard]] virtual auto WithBody(const nlohmann::json& body) -> ApiResponseExpectation& = 0;
        [[nodiscard]] virtual auto WithoutBody() -> ApiResponseExpectation& = 0;
    };
} // namespace UKControllerPluginTest
