#pragma once

namespace UKControllerPluginTest {
    class ApiRequestExpectation;

    /**
     * An interface for expecting a particular URI to be called.
     */
    class ApiUriExpectation
    {
        public:
        [[nodiscard]] virtual auto To(const std::string& uri) -> ApiRequestExpectation& = 0;
    };
} // namespace UKControllerPluginTest
