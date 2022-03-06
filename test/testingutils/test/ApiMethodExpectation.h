#pragma once

namespace UKControllerPluginTest {
    class ApiUriExpectation;

    /**
     * An interface for expecting API methods to be called.
     */
    class ApiMethodExpectation
    {
        public:
        virtual ~ApiMethodExpectation() = default;
        [[nodiscard]] virtual auto Get() -> ApiUriExpectation& = 0;
        [[nodiscard]] virtual auto Post() -> ApiUriExpectation& = 0;
        [[nodiscard]] virtual auto Put() -> ApiUriExpectation& = 0;
        [[nodiscard]] virtual auto Patch() -> ApiUriExpectation& = 0;
        [[nodiscard]] virtual auto Delete() -> ApiUriExpectation& = 0;
    };
} // namespace UKControllerPluginTest
