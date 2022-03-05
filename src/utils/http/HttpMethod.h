#pragma once

namespace UKControllerPluginUtils::Http {
    /**
     * Wrapper around cURL HTTP methods
     */
    class HttpMethod
    {
        public:
        [[nodiscard]] static auto Get() -> HttpMethod;
        [[nodiscard]] static auto Post() -> HttpMethod;
        [[nodiscard]] static auto Put() -> HttpMethod;
        [[nodiscard]] static auto Patch() -> HttpMethod;
        [[nodiscard]] static auto Delete() -> HttpMethod;
        [[nodiscard]] operator std::string() const;
        [[nodiscard]] auto operator==(const HttpMethod& compare) const -> bool;

        protected:
        HttpMethod(std::string method);

        private:
        std::string method;
    };
} // namespace UKControllerPluginUtils::Http
